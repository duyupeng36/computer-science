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




