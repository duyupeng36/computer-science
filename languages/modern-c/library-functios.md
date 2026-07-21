# 库函数

C 标准分为两部分：**C 语言本身**(语法、关键字) 和 **C 标准库**(提供常用功能的函数集合)

> [!TIP]
> C 标准库中的基本工具实现了我们在日常编程中需要的功能，并且我们需要清楚的接口和语义来确保可移植性

## C标准库的通用性质

C 库函数的目标大致分为两类：**平台抽象层** 和 **基础工具**

> [!TIP]
> **平台抽象层**: 这些函数隐藏了底层平台的具体实现细节。比如 `puts` 需要知道如何向"终端输出"写数据，这涉及到操作系统甚至处理器相关的知识。这些实现超出了大多数 C 程序员的知识范围，所以由库来提供。
>
> **基础工具**: 这些函数实现了 C 编程中经常出现的任务（比如 `strtod`），需要固定的接口、相对高效的实现、以及充分的测试

C 库有很多函数，**头文件(header)** 把相关接口组织在一起。使用时通过 `#include` 引入，比如

```c
#include <stdio.h>    // 输入输出
#include <stdlib.h>   // 基本工具函数
#include <string.h>   // 字符串处理
#include <math.h>     // 数学函数
```

下表列出了 C 程序常用的头文件

| 头文件        | 用途                                    |
| ------------- | --------------------------------------- |
| `<stdio.h>`   | 输入输出（printf, scanf, fopen...）     |
| `<stdlib.h>`  | 通用工具（malloc, exit, strtod...）     |
| `<string.h>`  | 字符串处理（strlen, strcpy, memcpy...） |
| `<math.h>`    | 数学函数（sin, cos, sqrt, pow...）      |
| `<time.h>`    | 时间处理（time, strftime...）           |
| `<assert.h>`  | 断言（assert）                          |
| `<ctype.h>`   | 字符分类（isdigit, toupper...）         |
| `<errno.h>`   | 错误码（errno）                         |
| `<stdbool.h>` | bool 类型（C23 之前需要）               |
| `<stdint.h>`  | 固定宽度整数类型                        |
| `<float.h>`   | 浮点类型属性                            |
| `<limits.h>`  | 整数类型属性                            |

C 库的大多数 **接口** 都是以函数形式提供，但可以选择宏来实现。C 库中有一种类似函数的宏，称为 **函数式宏**。例如

```c
#define putchar(c) putc(c, stdout)
```

> [!TIP]
> 无论是之前介绍的对象式宏还是函数式宏，它们都只是 **文本替换**，所以如果传入有副作用的表达式（比如 `putchar(x++)`），可能会被展开多次导致问题。
>
> 也就是说： **不要向宏或函数传递有副作用的表达式**

一些接口的参数或返回值是指针类型。目前我们还没完全掌握指针，但在大多数情况下可以传入已知指针或 `nullptr`。**指针作为返回值通常只用于表示错误条件**。

也就是说，C 库函数通过返回特殊值来表示失败。不同函数的错误表示方式不同，主要有以下几种模式

| 失败返回值 | 测试方式      | 典型案例     | 示例                 |
| ---------- | ------------- | ------------ | -------------------- |
| 空指针     | `!value`        | 其他值有效   | fopen                |
| 特殊错误码 | `value == code` | 其他值有效   | puts, strtod, fclose |
| 非零值     | `value`         | 否则无需     | fgetpos, fsetpos     |
| 特殊成功码 | `value != code` | 区分失败情况 | —                    |
| 负值       | `value < 0`     | 正值是计数   | printf               |

例如，检查 `puts` 函数输出是否遇见错误： 这里 `puts` 失败时返回 EOF，`perror` 提供错误诊断，`exit` 终止程序

```c
if (puts("hello world") == EOF) {
    perror("can't output to terminal:");
    exit(EXIT_FAILURE);
}
```

> [!TIP]
> 使用 C 库的三个准则
>
> 1. 失败总是可能的 — 不要忽视错误
> 2. 检查库函数的返回值 — 用于检测错误
> 3. 快速失败 — 程序立即崩溃比隐藏 bug 更好

C 还有一个跟踪错误的状态变量 `errno`。`perror` 在底层使用它来提供诊断信息。如果函数失败后可以恢复，需要确保重置错误状态：

```c
void puts_safe(char const s[static 1]) {
    static bool failed = false;
    if (!failed && puts(s) == EOF) {
        perror("can't output to terminal:");
        failed = true;
        errno = 0;  // 重置错误状态
    }
}
```

C 库中的很多函数才参数不一致式容易产生 **缓冲区溢出**，这是导致很多安全漏洞的根源。C11 标准开始努力解决这个问题

+ 弃用或移除一些危险的即可
+ 增加了可选的边界检查接口。这些边界检查接口通常是在原函数名后加 `_s` 后缀，如 `printf_s`、`fopen_s`。

> [!TIP]
> 不要使用 `_s` 后缀给自己的函数命名，这是保留的

想要使用边界检查接口需要两个宏

- `__STDC_LIB_EXT1__` — 检查平台是否支持 Annex K
- `__STDC_WANT_LIB_EXT1__` — 启用边界检查接口（必须在 `include` 之前定义）

```c
#if !__STDC_LIB_EXT1__
error "This code needs bounds checking interface Annex K"
#endif
#define __STDC_WANT_LIB_EXT1__ 1

#include <stdio.h>

/* 之后可以使用 printf_s 等安全版本 */
```

> [!WARNING]
> 注意： Annex K 是可选特性，很多现代平台选择不支持它。研究甚至表明这些接口带来的问题比解决的还多。

### 平台前置条件

可移植性是 C 的重要目标，但有时我们不得不依赖平台特定的特性。这时应该明确标识代码的前提条件。

> [!TIP]
> 如果执行平台不满足前提条件，必须中止编译

使用预处理器条件来实现

```C
#if !__STDC_LIB_EXT1__
error "This code needs bounds checking interface Annex K"
#endif
```

- `#if ... #endif` — 预处理器条件块
- `#error` — 条件为真时中止编译并输出错误信息
- `#warning` — 条件为真时继续编译但输出警告

> [!TIP]
> 预处理器条件中只能使用宏和整数字面量。对于未知的标识符会被视为 $0$

下表列出了一些特殊操作符，它们用于查询编译器能力

| 操作符            | 参数         | 作用               |
|-------------------|--------------|--------------------|
| `defined`           | 宏名         | 检查宏是否定义     |
| `__has_include`     | 头文件名     | 检查头文件是否存在 |
| `__has_embed`       | 二进制文件名 | 检查二进制文件     |
| `__has_c_attribute` | 属性名       | 检查属性支持       |

`defined` 还有几个快捷写法

```c
#ifdef X          // 等价于 #if defined(X)
#ifndef X         // 等价于 #if !defined(X)
#elifdef X        // C23: 等价于 #elif defined(X)
#elifndef X       // C23: 等价于 #elif !defined(X)
```

对于编译时但非预处理阶段能检查的条件，使用 `static_assert`

```c
static_assert(sizeof(double) == sizeof(long double),
            "Extra precision needed for convergence.");
```

> [!TIP]
> C23 之前关键字是 `_Static_assert`，`static_assert` 是 `<assert.h>` 中的宏

## 整数算术

C 语言已经用运算符定义了大部分整数算术功能，C 库补充了一些没有直接运算符表示或需要特殊处理的情况。大多数函数是 C23 新增的。

### abs 和 div 函数族

在头文件 `<stdlib.h>` 中提供了 `abs` 和 `div` 函数族。C 没有求有符号整数绝对值的运算符，所以提供了 `abs` 系列函数

```c
int abs(int x);
long labs(long x);
long long llabs(long long x);
```

> [!TIP]
> 为什么需要这个函数？因为手写的表达式 `(x < 0) ? -x : x` 会对 `x` 求值两次。

函数 `div` 返回的结果是一个结构体有 `quot`(商)和 `rem`(余数) 两个成员

```c
div_t div(int numer, int denom)
ldiv_t ldiv(long int numer, long int denom)
lldiv_t lldiv(long long numer, long long denom);
```

```c
auto res = div(x, y);
printf("%d/%d is %d, remainder %d\n", x, y, res.quot, res.rem);
```

> [!TIP]
> `auto` 关键字在 C23 中被重新定义为 **自动推导类型**

现代编译器能很好地将商和余数操作融合成单条指令，所以这些接口现在用处不大。

> [!WARNING]
> 注意： 除法对大多数值是良定义的，但除以 `0` 和 `INT_MIN / -1` 等组合是未定义的。

### 溢出检查函数(C23)

加法、减法、乘法可能发生溢出。C23 引入了三个类型泛型函数来安全检测溢出：

```c
#include <stdckdint.h>

bool ckd_add(type* result, type x, type y);  // result = x + y
bool ckd_sub(type* result, type x, type y);  // result = x - y
bool ckd_mul(type* result, type x, type y);  // result = x * y
```

- 第一个参数(`result`)是指针，用于存放结果
- 返回值：`true` 表示溢出，`false` 表示未溢出

> [!TIP]
> **无论是否溢出，操作都有效**——结果是正确结果的低位部分

下面的示例代码演示了无符号整数的溢出

```c
unsigned result = 0;
bool overflow = ckd_add(&result, UINT_MAX, UINT_MAX);
printf("Overflow flag %s, result %x\n", (overflow ? "true" : "false"), result);
// 输出：Overflow flag true, result fffffffe
// 数学结果是 2 * UINT_MAX，但 result 只保留低位：UINT_MAX - 1
```

另一个示例代码片段演示了 **有符号溢出**

```c
signed result = 0;
bool overflow = ckd_add(&result, -INT_MAX, -INT_MAX);
printf("Overflow flag %s, result %x\n",
        (overflow ? "true" : "false"),
        result);
// 数学结果是 2×INT_MIN + 2（负数），但 result = 2（正数）
// overflow = true
```

> [!WARNING]
> 直接写 `-INT_MAX + -INT_MAX` 是未定义行为，但 `ckd_add` 是良定义的

### 位操作函数(C23)

这些函数接受无符号整数，返回各种位属性：

| 函数                         | 描述                           | 特殊情况                   |
| ---------------------------- | ------------------------------ | -------------------------- |
| `stdc_bit_ceil(x)`           | $2^{\lceil \log_{2} x\rceil}$   | $x=0$ 返回 $1$，$x \gt \frac{m}{2}$ 返回 $0$ |
| `stdc_bit_floor(x)`          | $2^{\lfloor \log_{2} x\rfloor}$ | $x=0$ 返回 $0$                 |
| `stdc_bit_width(x)`          | $1+\lfloor \log_{2} x\rfloor$   | $x=0$ 返回 $0$                 |
| `stdc_count_ones(x)`         | $1$ 的个数                     | 类型无关                   |
| `stdc_has_single_bit(x)`     | $x$ 是否是 2 的幂              | 类型无关                   |
| `stdc_first_trailing_one(x)` | 最低位 $1$ 的位置 $+1$         | $x=0$ 返回 $0$                 |
| `stdc_trailing_zeros(x)`     | 尾部 $0$ 的个数                | 类型相关                   |

> [!TIP]
> 重要特性：
>
> - 所有函数对所有参数值都有定义好的结果
> - 优先使用描述性名称的函数（如 `stdc_count_ones`）提高可读性
> - 某些函数结果与参数类型无关（如 `stdc_bit_width`），优先使用
> - 某些函数结果与类型宽度相关（如 `stdc_leading_zeros`），尽量避免使用

## 数值计算

数值函数来自 `<math.h>`，但使用 `<tgmath.h>` 中的泛型宏更方便。这些宏会根据参数类型自动分派到正确的函数

```c
#include <tgmath.h>

double x = sin(3.14);       // 调用 double 版本
float y = sin(3.14f);       // 调用 float 版本
long double z = sin(3.14L); // 调用 long double 版本
```

三角函数：

| 函数                  | 描述                     |
| --------------------- | ------------------------ |
| `sin, cos, tan`       | 正弦、余弦、正切         |
| `asin, acos, atan`    | 反三角函数               |
| `atan2(y, x)`         | 双参数反正切（返回象限） |
| `sinh, cosh, tanh`    | 双曲函数                 |
| `asinh, acosh, atanh` | 反双曲函数               |

指数和对数：

| 函数       | 描述                                  |
| ---------- | ------------------------------------- |
| `exp(x)`   | $e^x$                                 |
| `exp2(x)`  | $2^x$                                 |
| `expm1(x)` | $e^x - 1$（$x$ 接近 $0$ 时更精确）    |
| `log(x)`   | $\ln x$                               |
| `log2(x)`  | $\log_{2}x$                           |
| `log10(x)` | $log_{10}x$                           |
| `log1p(x)` | $\ln (1+x)$ （$x$ 接近 $0$ 时更精确） |

幂运算：

| 函数          | 描述                           |
| ------------- | ------------------------------ |
| `pow(x, y)`   | $x^y$                          |
| `sqrt(x)`     | $\sqrt{x}$                     |
| `cbrt(x)`     | $\sqrt[3]{x}$                  |
| `hypot(x, y)` | $\sqrt{x^2 + y^2}$（避免溢出） |

取整：

| 函数           | 描述                             |
| -------------- | -------------------------------- |
| `ceil(x)`      | 向上取整 $\lceil x\rceil$        |
| `floor(x)`     | 向下取整 $\lfloor x\rfloor$      |
| `trunc(x)`     | 向零取整                         |
| `round(x)`     | 四舍五入                         |
| `nearbyint(x)` | 使用当前舍入模式的最近整数       |
| `rint(x)`      | 类似 `nearbyint`，但可能触发异常 |

其他：

| 函数             | 描述                                 |
| ---------------- | ------------------------------------ |
| `fabs(x)`        | 浮点绝对值                           |
| `fmod(x, y)`     | 浮点除法余数                         |
| `fma(x, y, z)`   | $x\cdot y + z$（融合乘加，精度更高） |
| `copysign(x, y)` | 将 $y$ 的符号复制到 $x$              |
| `fdim(x, y)`     | 正差值，`max(x-y, 0)`                |
| `fmax, fmin`     | 浮点最大/最小值                      |

分类函数（返回 int/bool）：

| 函数            | 描述       |
| --------------- | ---------- |
| `isfinite(x)`   | 是否有限   |
| `isinf(x)`      | 是否无穷   |
| `isnan(x)`      | 是否 `NaN` |
| `isnormal(x)`   | 是否正规数 |
| `signbit(x)`    | 是否负数   |
| `fpclassify(x)` | 分类浮点值 |

> [!TIP]
> 重要说明：
>
> 1. 很多函数可以直接使用处理器特定指令实现，比如快速近似的 `sqrt`、`sin`，或者硬件级别的 `fma`。不要试图自己重新实现这些函数，手写代码通常更慢且精度更低。
> 2. `log1p` 和 `expm1` 在 `x` 接近 $0$ 时比 `log(1+x)` 和 `exp(x)-1` 更精确，避免了灾难性抵消。
> 3. `hypot(x, y)` 计算 $\sqrt{(x^2+y^2)}$ 时会小心处理溢出，比手写 `sqrt(x * x + y * y)` 更安全


## 标准 IO

头文件 `<stdio.h>` 是 C 程序最常使用的功能之一。我们在 [快速入门](getting-started.md) 中简单介绍过用于格式化输出的 `printf` 函数和格式化输入的 `scanf` 函数。下面我们开始学习其他输入输出函数

### 无格式文本输出

函数 `puts` 用于输出一个字符串，它会在输出完成之后追加一个 `'\n'`。比 `puts` 更基本的函数称为 `putchar`，它用于输出一个字符。它们的函数原型如下

```c
int putchar(int c);                     // 输出一个字符
int puts(char const s[static 1]);       // 输出一个字符串
```

> [!TIP]
> `putchar` 参数类型 `int` 是历史遗留，实际只用到字符部分。成功输出返回参数 `c` 的值，失败返回 `EOF`(负值)
>
> `puts` 参数类型 `char const [static 1]`。成功输出返回 **输出的字节数**，失败返回 `EOF`

显然是，`puts` 函数可以使用 `putchar` 实现；`puts` 的实现可能是这样的

```c
int puts(char const s[static 1]) {
    int result = 0;
    for (size_t i = 0; s[i]; ++i) {
        if (putchar(s[i]) == EOF) {
            return EOF;
        }
        ++result;
    }
    // 追加一个 \n
    if (putchar('\n') == EOF) {
        return EOF;
    }
    ++result;
    return result;
}
```

### 流(stream) 和 FILE 类型

到目前为止，我们输出目标始终是终端(标准输出)。C 标准库提供了 `FILE*` 类型，它是对 **流** 的一种抽象。`FILE*` 可以指向文件或者其他输出目标

> [!TIP]
> **流是对 I/O 设备的抽象**。 你不需要知道底层是磁盘、终端、网络还是管道——C 标准库把它们统一成一个概念：**字节序列**。也就是说，**程序通过流与物理设备打交道**
>
> | 类型         | 含义                                 | 典型例子                     |
> | ------------ | ------------------------------------ | ---------------------------- |
> | **文本流**   | 字节序列被组织成行，每行以 `\n` 结尾 | 源代码文件、配置文件         |
> | **二进制流** | 原始字节，不做任何转换               | 图片、可执行文件、序列化数据 |
>
> `FILE` 类型是 C 库定义的一个结构体。注意，我们需要把它当作 **不透明** 类型，只能通过函数接口操作它，不要依赖于 `FILE` 结构体的实现细节


函数 `fputc` 和 `fputs` 可以将字符输出到任意流中

```C
int fputc(int c, FILE* stream); // 向 stream 写入一个字符
int fputs(char const s[static 1], FILE* stream); // 向 stream 写入字符串
```

> [!WARNING]
> 请注意，`fputs` 与 `puts` 存在些许差异：`puts` 会追加输出 `'\n'`，但是 `fputs` 不会

任何程序在执行时会打开两个标准输出流，分别是

+ `stdout`：标准输出流，通常连接到终端
+ `stderr`: 标准错误流，也连接到终端，用于区分"正常输出"和"紧急输出"

显然，之前的 `putchar` 和 `puts` 的实现都是通过 `fputc` 和 `fputs` 实现的

```c
// putchar 的实现
int putchar(int c) {
    return fputc(c, stdout);
}

int puts(char const s[static 1]) {
    int result = fputs(s, stdout);
    if (result == EOF) {
        return EOF;
    }
    if (fputc('\n', stdout) == EOF) {
        return EOF;
    }
    ++result; // 补充 \n 输出是占用的字节数
    return 0;
}
```

C 程序最重要的 IO 操作就是 **文件IO操作**。想要操作文件，就需要先使用 `fopen` 打开文件，其函数原型如下

```c
FILE* fopen(char const path[static 1], char const mode[static 1]);
```

- 参数 `path` 是需要打开文件的**路径**。支持 **绝对路径** 和 **相对路径**
- 参数 `mode` 是打开文件的模式：指定怎样操作文件
- 返回值：文件成功打开返回一个 `FILE*` 类型的指针；否则，返回 `nullptr`

下面演示程序打开一个文件并追加一个字符串

```c
#include <stdio.h>  // 不要忘记

int main(int argc, char* argv[argc + 1]) {
    FILE* logfile = fopen("mylog.txt", "a");
    if (!logfile) {
        perror("fopen failed");
        return 1;
    }
    fputs("feeling fine today\n", logfile);
    return 0;
}
```

参数 `mode` 指定**文件打开模式**。文件打开模式的字符由 **模式** 和 **修饰符** 两部分组成。其中，模式包含 $3$ 种

| 模式 | 含义 | 文件状态                 |
|------|------|--------------------------|
| `"r"`  | 只读 | 文件必须存在，位置在开头 |
| `"w"`  | 只写 | 清空文件（或创建新文件） |
| `"a"`  | 追加 | 保留内容，位置在末尾     |

修饰符包含 $3$ 种。

| 修饰符 | 含义                              |
|--------|-----------------------------------|
| `"+"`    | 同时读写                          |
| `"b"`    | 二进制模式（否则是文本模式）      |
| `"x"`    | 排他创建（文件已存在则失败，C11） |

> [!WARNING]
> 注意：**模式** 在参数 `mode` 中必须唯一的，配合 **修饰符** 可以组合处各种模式
>
> 常见组合：`"r+"`(读文件附加写操作), `"w+"`(写文件附件读操作), `"a+"`(追加文件附加读操作), `"rb"`(读二进制), `"wb"`(写二进制), `"ab"`(追加二进制) 等。

一个打开的流会占据系统资源；当程序不在使用时需要**及时关闭它**从而是否系统资源。函数 `fclose` 用于关闭流。如果想要复用已经打开的流(关联到其他文件或设备)，则可以使用 `freopen` 函数。C 标准库为流提供了一个 **缓冲层**(不立即写入文件，等后续批量写入)；如果想要立即将缓冲层中的数据写入文件，则可以使用 `fflush` 函数强制刷新缓冲层

```c
FILE* freopen(char const path[static 1], char const mode[static 1], FILE* stream);  // 通常用于重定向标准流
int fclose(FILE* fp); // 关闭已打开的文件
int fflush(FILE* stream); // 刷新缓冲区
```

### 文本 IO

流的输出通常是 **缓冲的**：IO 系统可能延迟实际写入以提高效率。当我们使用函数 `fclose` 关闭流时，所有缓冲区会被刷新(执行实际写入操作)。在需要立即刷新缓冲区或者没准备关闭文件但需要确保数据到达目标时，可以使用函数 `fflush` 强制刷新缓冲区

C 标准输出提供了 $3$ 种缓冲模式，在 `<stdio.h>` 中定义了 $3$ 个宏分别表示

| 宏       | 含义                                                |
| -------- | --------------------------------------------------- |
| `_IONBF` | 无缓冲: 立即刷新（`stderr` 默认）                   |
| `_IOLBF` | 行缓冲: 遇到 `\n` 就刷新（`stdout` 关联终端时默认） |
| `_IOFBF` | 全缓冲: 缓冲区满才刷新                              |

> [!TIP]
> 对于文本流，最常使用的缓冲模式是 **行缓冲**。所以，`puts` 的输出通常立即出现在终端，而 `fputs` 需要等到遇见 `\n` 时才会输出

**文本输入和输出会进行转换数据**。这是因为 **内部和外部的字符表示不一定相同**。目前，有许多不同的字符编码正在被使用；如果可以的话，C 库函数负责进行正确的转换。

不同平台用不同的方式表示换行，例如，Linux 系统使用 `\n`；Windows 系统使用 `\r\n`; MacOs 使用 `\r` 表示换行

> [!TIP]
> C 标准库用 `\n` 抽象了行尾编码差异，无论平台如何都能正确工作

### 格式化输出

函数 `fprintf` 与 `printf` 类似，但是 `fprintf` 可以指定任意输出流

```c
int printf(char const format[static 1], ...);
int fprintf(FILE* stream, char const format[static 1], ...);
```

函数原型中的 `...` 表示可变参数(头文件 `<stdarg.h>` 提供了访问可变参数的接口)；请注意：参数个数必须与 **格式说明符** 一一对应。格式说明符语法： `%[FF][WW][.PP][LL]SS`

| 部分 | 含义          | 是否必须 |
|------|---------------|----------|
| `FF`   | 标志（flags） | 可选     |
| `WW`   | 最小字段宽度  | 可选     |
| `PP`   | 精度          | 可选     |
| `LL`   | 类型修饰符    | 可选     |
| `SS`   | 转换说明符    | 必须     |


下表列出了常用的转换说明符

| 说明符        | 含义       | 适用类型     |
| ------------- | ---------- | ------------ |
| `%d` 或 `%i`  | 十进制     | 有符号整数   |
| `%u`          | 十进制     | 无符号整数   |
| `%b`          | 二进制     | 无符号整数   |
| `%o`          | 八进制     | 无符号整数   |
| `%x` 或 `%X ` | 十六进制   | 无符号整数   |
| `%f` 或 `%F`  | 定点       | 浮点         |
| `%e` 或 `%E`  | 科学计数法 | 浮点         |
| `%g` 或 `%G`  | 通用格式   | 浮点         |
| `%c`          | 字符       | 整数         |
| `%s`          | 字符串     | 字符串       |
| `%p`          | 地址       | `void*` 指针 |
| `%%`          | 字面 %     | 无参数       |

> [!TIP]
>
> + 打印整数用 `%d`（有符号）和 `%u`（无符号）
> + 打印位模式用 `%b`（小数据）或 `%x`（大数据）
> + 打印浮点数用 `%g`（通用格式）

下表列出了常用的类型修饰符(重要!!)

| 修饰符 | 类型              |
| ------ | ----------------- |
| `hh`   | `char`            |
| `h`    | `short`           |
| （无） | `int`, `unsigned` |
| `l`    | `long`            |
| `ll`   | `long long`       |
| `z`    | `size_t`          |
| `t`    | `ptrdiff_t`       |
| `L`    | `long double`     |

> [!WARNING]
> 用错误的修饰符是未定义行为！ 好的编译器会对此发出警告，一定要认真对待


下表列出了 C 标准定义的 $5$ 种格式标志

| 标志 | 含义         | 示例                        |
| ---- | ------------ | --------------------------- |
| `+`  | 正数也显示 + | `printf("%+d", 5) → +5`     |
| `-`  | 左对齐       | `printf("%-10d", 5) → 5`    |
| `0`  | 零填充       | `printf("%05d", 5) → 00005` |
| `#`  | 替代形式     | `printf("%#x", 255) → 0xff` |
| ` `  | 正数前加空格 | `printf("% d", 5) →  5`     |

### 无格式文本输入

C 标准提供了 $3$  个无格式文本的输入函数

```c
int fgetc(FILE* stream);                                    // 读取一个字符
char* fgets(char s[restrict], int n, FILE* restrict stream); // 读取一行
int getchar(void);                                          // 从 stdin 读取一个字符
```

> [!TIP]
> 标准输入流 `stdin` 是每个程序执行时都会打开的，通常连接到键盘

函数 `fgetc` 从 `stream` 读取一个字符，它使用 `int` 作为返回值是为了能够使用 `EOF` 表示错误状态。`EOF` 是一个负值，`char` 类型无法表示所有有效字符加一个特殊错误值，所以返回 `int`

> [!TIP]
> `EOF` 表示文件结束。想要检测文件结束只能在一次失败的读取之后才能检测。注意，单纯的检测 `EOF` 是不够的，它无法确认是因为到达文件末尾而发生的错误

想要真正的检测出文件结束还需要配合函数 `feof` 来确认

```c
if (feof(stream)) {
    // 文件到达末尾
} else {
    // 发生了其他读取错误
}
```

函数 `fgets` 从指定的 `stream` 中读取 $n$ 字节到 `s` 中。它可以使用 `fgetc` 实现。例如

```c
char* fgets(char s[restrict], int n, FILE* restrict stream) {
    if (!stream) {
        return nullptr;
    }
    if (!n) {
        return s;
    }
    /* 最多读取 n - 1 个字符 */

    for (size_t i = 0; i < n - 1; ++i) {
        int val = fgetc(stream);
        switch(val) {
            case EOF:
                if (feof(stream)) {
                    // 文件结束
                    s[i] = 0;
                    return s;
                } else {
                    // 读取错误
                    return nullptr;
                }
            // 遇到换行符停止
            case '\n':
                s[i] = val;
                s[i+1] = 0;
                return s;
            // 其他字符直接赋值
            default:
                s[i] = val;
        }
    }
    s[n-1] = 0;
    return s;
}
```

> [!TIP]
> 函数 `fgets` 成功完成后，`fgets()` 应返回 `s`。如果失败，返回 `nullptr`

### 示例程序: 简易 cat 程序

这个示例程序我们实现一个简单的 cat 程序：读取文件中的内容并写入标准输出

```c title="cat.c" linenums="1"
/* cat.c - 将文件内容写入标准输出 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

enum {BUFF_SIZE = 32};


int main(int argc, char* argv[argc + 1]) {
    if (argc < 1) {
        fprintf(stderr, "Usage: %s <filename> [<filename>...]", argv[0]);
        return EXIT_FAILURE;
    }
    int ret = EXIT_FAILURE;
    char buffer[BUFF_SIZE] = {};
    for (int i = 1; i < argc; ++i) {
        FILE* instream = fopen(argv[i], "r");
        if (instream) {
            while (fgets(buffer, BUFF_SIZE, instream)) {
                fputs(buffer, stdout);
            }
            fclose(instream);
            ret = EXIT_SUCCESS;
        } else {
            fprintf(stderr, "Could not open %s: ", argv[i]);
            perror(0);
            errno = 0;
        }
    }
    return ret;
}
```

> [!TIP]
> 这里我们设置缓冲大小为 $32$ 个字符。即使程序遇到超过 $31$ 个字符的行也能正常工作，因为 `fgets` 每次最多读取 $BUFF_SIZE - 1$ 个字符，循环会继续读取剩余部分


## 字符串处理和转换

C 中字符串处理要面对一个现实：**源代码和执行环境可能使用不同的字符编码**。关键是使用与编码无关的接口。

### 字符分类和转换

在 `<ctype.h>` 头文件中提供了 **分类** 和 **转换** 两类函数。它们分别是

| 名称       | 含义                 | C 语言环境                       | 已扩展 |
| ---------- | -------------------- | -------------------------------- | ------ |
| `islower`  | 小写字母             | `a-z`                            | 是     |
| `isupper`  | 大写字母             | `A-Z`                            | 是     |
| `isalpha`  | 字母                 | `A-Z, a-z`                       | 是     |
| `isdigit`  | 十进制数字           | `0-9`                            | 是     |
| `isxdigit` | 十六进制数字         | `0-9, A-F, a-f`                  | 否     |
| `isalnum`  | 字母或数字           | `A-Z, a-z, 0-9`                  | 否     |
| `isspace`  | 空白字符             | `" ", \t, \v, \n, \f, \r`        | 是     |
| `ispunct`  | 标点符号             | 除空格、字母、数字外的可打印字符 | 是     |
| `isprint`  | 可打印字符           | `0x20-0x7E`                      | 是     |
| `isgraph`  | 可见字符（不含空格） | `0x21-0x7E`                      | 是     |
| `iscntrl`  | 控制字符             | `0x00-0x1F, 0x7F`                | 是     |
| `isblank`  | 空白字符             | `" ", \t`                        | 是     |

此外，还有两个转换函数 `toupper` 和 `tolower`

```c
int toupper(int c);  // 转大写，非字母不变
int tolower(int c);  // 转小写，非字母不变
```

> [!WARNING]
> 注意：所有这些函数的参数和返回值都是 `int`，这是历史原因

### 转义字符

> [!TIP]
> **转义字符** 是用于表示无法通过键盘输入或者在 C 程序中无法直接表示的字符。

C 标准提供了两种类型的转义字符：**字符型转义字符** 和 **数值型转义字符**。其中下表列出了 C 标准中可用的字符型转义字符

| 转义 | 含义     |
|------|----------|
| `\'`   | 单引号   |
| `\"`   | 双引号   |
| `\?`   | 问号     |
| `\\`   | 反斜杠   |
| `\a`   | 响铃     |
| `\b`   | 退格     |
| `\f`   | 换页     |
| `\n`   | 换行     |
| `\r`   | 回车     |
| `\t`   | 水平制表 |
| `\v`   | 垂直制表 |

数值型转义字符可以直接使用 **数值编码字符** 表示字符：可选择使用 **八进制** 或 **十六进制**

+ 八进制：`'\037'`（最多 $3$ 位八进制数字）
+ 十六进制：`'\xFFFF'`（`x` 后面的所有十六进制数字，注意这里的 `x` 是必须的）

> [!WARNING]
>  注意陷阱： `"\xdeBruyn"` 不等于 `"\xde"` `"Bruyn"`，而是 `"\xdeB"` 和 `"ruyn"`——**十六进制转义序列会贪婪地吸收后续的十六进制数字**

> [!TIP]
> 数值编码字符的解释取决于执行字符集。所以尽量避免使用数值编码，不是完全可移植的

### 字符串转数值函数

在 `<stdlib.h>` 中提供了丰富的字符串到数值的转换函数。下表列出了常用的数值转换函数

| 函数      | 返回类型           |
|-----------|--------------------|
| `strtod`    | `double`             |
| `strtof`    | `float`              |
| `strtold`   | `long double`        |
| `strtol`    | `long`               |
| `strtoll`   | `long long`          |
| `strtoul`   | `unsigned long`      |
| `strtoull`  | `unsigned long long` |
| `strtoimax` | `intmax_t`           |
| `strtoumax` | `uintmax_t`          |

> [!TIP]
> `strtol` 系列的接口需要接收 $3$ 参数
>
>
> ```c
> unsigned long strtoul(char const nptr[restrict], char** restrict endptr, int base);
> ```
>
> - `nptr` — 要转换的字符串
> - `endptr` — 指向剩余未处理部分的指针（目前可以传 0/nullptr）
> - `base` — 进制基数

参数 `base` 的常用值包括：

| base | 含义                                                               |
|------|--------------------------------------------------------------------|
| `0`    | 自动检测（根据前缀：`0x`=十六进制，`0b`=二进制，`0`=八进制，否则十进制） |
| `2`    | 二进制                                                             |
| `8`    | 八进制                                                             |
| `10`   | 十进制                                                             |
| `16`   | 十六进制                                                           |

下面的示例函数 `hexatridecimal` 将字母数字字符转换为 $36$ 进制的值(`0-35`)，大小写字母映射到相同值。

```c
static_assert('z'-'a' == 25, "alphabetic characters not contiguous");

unsigned hexatridecimal(int a) {
    if (isdigit(a)) {
        return a - '0';         // '0'..'9' => 0..9
    } else {
        a = toupper(a);
        return (isupper(a)) ? 10 + (a - 'A') : -1;  // 'A'..'Z' => 10..35
    }
}
```

在 `<string.h>` 中还有两个字符串搜索函数

```c
// 返回 s 开头包含在 accept 中的连续字符数
size_t strspn(char const s[static 1], char const accept[static 1]);
// 返回 s 开头不包含在 reject 中的连续字符数
size_t strcspn(char const s[static 1], char const reject[static 1]);
```

函数 `strspn` 计算字符串 `s` 开头有多少个连续字符在 `accept` 字符串中。函数 `strncspn` 则执行与 `strspn` 相反的功能(计算字符串 `s` 开头有多少个连续的字符不在 `reject` 字符串中)

## 时间

时间处理来自 `<time.h>`，有两大类时间：**日历时间** 和 **处理器时间**。下面我们首先看日历时间。C 库中用于处理日历时间的函数包括

```c
time_t time(time_t *t);                              // 获取当前时间戳
double difftime(time_t time1, time_t time0);         // 计算两个时间的差（秒）
time_t mktime(struct tm tm[1]);                      // struct tm 转 time_t
size_t strftime(char s[static 1], size_t max,
                char const format[static 1],
                struct tm const tm[static 1]);       // 格式化时间字符串
int timespec_get(struct timespec ts[static 1], int base);  // 获取高精度时间
```

`struct tm` 结构体用人类可读的方式来表示日历时间。该结构体涉及如下成员

| 成员     | 含义       | 起始值                   |
|----------|------------|--------------------------|
| `tm_sec`   | 秒         | `0-60`（60 是闰秒）        |
| `tm_min`   | 分钟       | `0-59`                     |
| `tm_hour`  | 小时       | `0-23`                     |
| `tm_mday`  | 日         | `1-31`（注意从1开始！）    |
| `tm_mon`   | 月         | `0-11`（0=一月）           |
| `tm_year`  | 年         | 需要加 `1900`              |
| `tm_wday`  | 星期几     | `0-6`（0=周日）            |
| `tm_yday`  | 年中第几天 | `0-365`                    |
| `tm_isdst` | 夏令时标志 | 正数=是，`0`=否，负数=未知 |

> [!WARNING]
> `struct tm` 结构成员的计数方式不统一。其中 `tm_mday` 从 $1$ 开始，其他的从 $0$ 开始

### mktime 函数

函数 `mktime` 会做下面三件事情，然后返回 `struct tm` 对应的 `time_t` 类型的时间

1. 规范化层级日期成员到各自范围
2. 设置 `tm_wday` 和 `tm_yday`
3. 如果 `tm_isdst` 为负，判断该日期是否处于夏令时

> [!TIP]
> `time_t` 类型表示的是线性时间戳
>
> - 线性时间尺度，适合计算
> - 值为 `0` 的时刻称为纪元（`epoch`），通常是 `1970`年 `1` 月 `1` 日 `0` 时 `0` 分 `0` 秒
> - 粒度通常是**秒**，但不保证
> - `difftime` 将两个 `time_t` 的差转换为秒（`double` 类型）

### 时间转换函数

下面的几个函数用于在 `time_t` 和 `struct tm` 之间进行转换

```c
// 已弃用（使用全局状态，不安全）
struct tm *gmtime(const time_t *timer);
struct tm *localtime(const time_t *timer);

// C23 安全版本
time_t timegm(struct tm *timeptr);
struct tm *gmtime_r(const time_t *timer, struct tm *buf);
struct tm *localtime_r(const time_t *timer, struct tm *buf);
```

函数 `gmtime`(global time)将 `time_t` 类型转换为 `struct tm` 类型(UTC 时间)。函数 `localtime` 将 `time_t` 类型转换为 `struct tm` 类型(本地时间)

> [!WARNING]
> 注意：函数 `gmtime` 和 `localtime` 使用了全局状态，因此，它们是不可重入的函数。在新程序中，我们应该使用它们对应的可重入版本 `gmtime_r` 和 `localtime_r`

函数 `gmtime_r` 和 `localtime_r` 是 `gmtime` 和 `localtime` 的可重入版本，它要求调用方自行提供 `struct tm` 对象用于存储转换结果

### 时间字符串格式化

函数 `strftime` 类似于 `printf`，但是 `%` 代码表示日期时间部分。下表列出了常用的时间代码

| 代码 | 含义             | 示例       |
|------|------------------|------------|
| `%Y`   | 四位年份         | `2026`       |
| `%m`   | 月份（01-12）    | `07`         |
| `%d`   | 日（01-31）      | `20`         |
| `%H`   | 小时（00-23）    | `14`         |
| `%M`   | 分钟（00-59）    | `30`         |
| `%S`   | 秒（00-60）      | `45`         |
| `%A`   | 星期全名         | `Sunday`     |
| `%a`   | 星期缩写         | `Sun`        |
| `%B`   | 月份全名         | `July`       |
| `%b`   | 月份缩写         | `Jul`        |
| `%F`   | 等价于 `%Y-%m-%d`  | `2026-07-20` |
| `%T`   | 等价于 `%H:%M:%S`  | `14:30:45`   |
| `%c`   | 首选日期时间表示 | 平台相关   |
| `%x`   | 首选日期表示     | 平台相关   |
| `%X`   | 首选时间表示     | 平台相关   |
| `%Z`   | 时区名           | `CST`        |
| `%%`   | 字面 `%`           | `%`          |


函数 `strftime` 的使用示例

```c
char buffer[100];
time_t now = time(0);       // 获取本地时间戳

struct tm local;
localtime_r(&now, &local);  // 转换为本地 struct tm

strftime(buffer, sizeof buffer, "%Y-%m-%d %H:%M:%S", &local); // 格式化
printf("现在是：%s\n", buffer);
```

### 高精度时间

类型 `time_t` 粒度通常只有秒。如果需要更高精度时间，需要使用函数 `timespce_get` 获取

```c
int timespec_get(struct timespec ts[static 1], int base);  // 获取高精度时间
```

类型 `struct timespec` 结构包含两个成员 `tv_sec` 和 `tv_nsec`；其中 `tv_sec` 是秒级精度，而 `tv_nsec` 是纳秒精度

```c
struct timespec ts;
timespec_get(&ts, TIME_UTC);
printf("秒：%ld，纳秒：%ld\n", ts.tv_sec, ts.tv_nsec);
```

### 处理器时间

函数 `clock` 用于获取程序使用的处理器时间，单位 `CLOCK_PER_SEC` 分之一秒。用于测量程序的运行时间

```c
clock_t clock(void);
```

## 运行时环境设置

### 获取环境变量

C 程序可以访问 **环境列表**，称为 **环境变量**

> [!TIP]
> 环境变量:一组 **名称-值对** 的字符串

函数 `getenv` 用于获取环境变量

```c
// 不安全
char* getenv(char const name[static 1]);
```

- 返回指向环境变量值的指针
- 如果变量不存在，返回 `nullptr`

注意，函数 `getenv` 直接使用了全局状态，不安全的。其安全版本是 `getenv_s`

```c
errno_t getenv_s(size_t * restrict len,
                char value[restrict],
                rsize_t maxsize,
                char const name[restrict]);
```

- 将环境变量的值复制到 `value` 缓冲区（最大 `maxsize` 字节）
- `len` 可用于获取实际需要的长度
- 返回 `0` 表示成功，非零表示失败或缓冲区不够


例如，下面的示例函数用于打印当前环境列表

```c
void printenv(char const name[static 1]) {
    if (getenv(name)) {
        char value[256] = { };
        if (getenv_s(nullptr, value, sizeof value, name)) {
            fprintf(stderr, "%s: value is longer than %zu\n",
                    name, sizeof value);
        } else {
            printf("%s=%s\n", name, value);
        }
    } else {
        fprintf(stderr, "%s not in environment\n", name);
    }
}
```

常见环境变量

| 变量   | 含义                 |
|--------|----------------------|
| `HOME`   | 用户主目录           |
| `PATH`   | 可执行文件搜索路径   |
| `LANG`   | 语言设置             |
| `LC_ALL` | 覆盖所有 locale 设置 |

### Locale 设置

程序启动时，C 强制 locale 为标准化的 `"C"` locale（基本是美式英语）。函数 `setlocale` 用于修改 locale

```c
#include <locale.h>

char* setlocale(int category, char const locale[static 1]);
```

第一个参数 `category` 的可选值在下表列出

| 类别        | 影响范围                       |
|-------------|--------------------------------|
| LC_ALL      | 所有类别                       |
| LC_COLLATE  | 字符串比较（strcoll, strxfrm） |
| LC_CTYPE    | 字符分类（<ctype.h>）          |
| LC_MONETARY | 货币格式                       |
| LC_NUMERIC  | 数字格式（小数点等）           |
| LC_TIME     | 时间格式（strftime）           |

第二个参数 `locale` C 标准只规定了两个有效值

- `""` 设置为系统默认
- `"C"` 标准美式英语
- `nullptr` 表示获取当前 `locale`

> [!TIP]
> 平台可能支持更多 `locale` 值（如 `"zh_CN.UTF-8"`），请查询平台文档

## 程序终止和断言

正常程序终止应该从 `main` 函数返回。在可能终止正常控制流的函数中，可以使用 `exit` 函数(在 `main` 函数中使用 `exit` 没有任何收益，直接使用 `return` 即可)

程序终止函数有下面 $4$ 个，按照严重程度排序

```c
[[noreturn]] void exit(int status);            // 正常终止，执行清理
[[noreturn]] void quick_exit(int status);      // 快速终止，执行快速清理
[[noreturn]] void _Exit(int status);           // 立即终止，只做平台清理
[[noreturn]] void abort(void);                 // 异常终止，不做清理
```

> [!WARNING]
> 除非必须禁止清理操作，否则不要使用 `exit` 以外的终止函数
>
> - `exit` — 执行所有注册的 `atexit` 处理器，刷新关闭文件
> - `quick_exit` — 执行 `at_quick_exit` 处理器，不执行 `atexit` 处理器
> - `_Exit` — 只做平台清理（如关闭文件），不执行任何应用处理器
> - `abort` — 不做清理，直接终止

程序终止时，可能需要执行一些清理动作。这些清理动作有 `atext` 和 `at_quick_exit` 函数注册

```c
int atexit(void func(void));           // 注册 exit 时执行的处理器
int at_quick_exit(void func(void));    // 注册 quick_exit 时执行的处理器
```

下面的示例代码片段演示 `atexit` 的用法

```c
void sayGoodBye(void) {
    if (errno) perror("terminating with error condition");
    fputs("Good Bye\n", stderr);
}

int main(int argc, char* argv[argc+1]) {
    atexit(sayGoodBye);
    // ... 程序代码 ...
}
```

注意：传给 `atexit` 的是函数名本身（没有括号），表示传递函数引用，不是调用它。

> [!TIP]
> 适用场景：
>
> - 释放资源
> - 写终止时间戳到日志
> - 打印诊断信息

### 断言

#### 编译时断言

C23 标准引入了 `static_assert` 关键字用于执行编译时断言

```c
static_assert(sizeof(测试条件, 提示信息);
```

> [!WARNING]
> 测试条件必须在编译时成立的条件。**条件为假时中止编译** 并输出提示信息

例如

```c
static_assert(sizeof(double) == sizeof(long double), "Extra precision needed for convergence.");
```

#### 运行时断言

头文件 `<assert.h>` 提供了一个宏 `assert(condition)`：如果运行时 `condition` 不成立，打印诊断信息并调用 `abort`。例如

```c
#include <assert.h>

int gcd(int a, int b) {
    assert(a > 0 && b > 0);  // 确保参数为正
    while (b) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}
```

使用尽可能多的 `assert` 来确认运行时属性。在调试阶段非常有用，能快速定位错误假设

> [!WARNING]
> **生产环境禁用 assert**: 在生产编译中使用 `NDEBUG` 禁用所有 `assert`
>
> - 编译时定义 `NDEBUG`，`assert` 就不会执行
> - 通常通过编译器命令行设置：`gcc -DNDEBUG ...`
> - 因为 `assert` 会调用 `abort`，不适合给用户看到
