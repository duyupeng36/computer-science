# 组织与文档

程序是一段描述性的文本，不仅要给编译器下指令，还要记录系统的行为意图。代码的组织和文档化需要在"提供上下文"和"陈述显而易见的事情"之间找到平衡

```c
/* 坏例子：魔术常量 + 无意义的注释 */
u = fun4you(u, i, 33, 28);    // ;)
++i;                           // incrementing i

/* 好例子：命名自解释 + 注释解释目的 */
/* 33 and 28 are suitable because they are coprime. */
u = nextApprox(u, i, 33, 28);
/* Theorem 3 ensures that we may move to the next step. */
++i;
```

由此引出四个层次的规则，按重要性排序

| 层次           | 关键词       | 说明                                  | 读者数量 |
| :------------- | :----------- | :------------------------------------ | :------- |
| 函数接口       | **what**     | 描述**做了什么**                      | 所有用户 |
| 接口注释       | **what for** | 记录函数的**目的**                    | 大多数   |
| 函数代码       | **how**      | 展示**如何组织**                      | 少数     |
| 代码注释       | **in which manner** | 解释实现的**具体方式**         | 极少数   |

> [!TIP]
> + **分离接口和实现**。C 语言通过两种源文件实现：头文件（`.h`）$\rightarrow$ 接口，翻译单元（`.c`）$\rightarrow$ 实现
> + **文档化接口；解释实现**。接口需要完整的文档，代码只需要解释不明显的想法

## 接口文档

C 语言没有内置的文档标准，但 [doxygen](https://www.doxygen.nl/) 已被广泛采用，它可以自动生成网页、PDF 手册、调用图等

> [!TIP]
> 彻底地文档化接口。即使不使用 doxygen 工具，也应使用它的语法来文档化接口

doxygen 常用关键字

| 关键字      | 说明                              |
| :---------- | :-------------------------------- |
| `@brief`    | 函数的简短摘要                    |
| `@param`    | 描述一个参数                      |
| `@a`        | 在文档中引用参数名                |
| `@f$...@f$` | 数学公式（LaTeX 语法）            |
| `@see`      | 交叉引用                          |
| `@file`     | 给整个文件一个说明                |
| `/**< ... */` | 行内文档，紧跟在成员后面        |

示例

```c
/**
 ** @brief use the Heron process to approximate @a a to the
 ** power of `1/k`
 **
 ** Or in other words this computes the @f$k^{th}@f$ root of @a a
 ** As a special feature, if @a k is `-1` it computes the
 ** multiplicative inverse of @a a.
 **
 ** @param a must be greater than `0.0`
 ** @param k should not be `0` and otherwise be between
 ** `DBL_MIN_EXP*FLT_RDXRDX` and
 ** `DBL_MAX_EXP*FLT_RDXRDX`.
 **
 ** @see FLT_RDXRDX
 **/
double heron(double a, signed k) [[__unsequenced__]];
```

### 头文件组织

将语义强关联的代码组织在一起。最常见的做法：把处理某个数据类型的所有函数放在同一个头文件中。典型的头文件结构如下

```c
#ifndef BRIAN_H
#define BRIAN_H 1
#include <time.h>

/** @file
 ** @brief Following Brian the Jay
 **/

typedef struct brian brian;
enum chap { sct, en, };
typedef enum chap chap;

struct brian {
    struct timespec ts; /**< point in time */
    unsigned counter;   /**< wealth        */
    chap masterof;      /**< occupation    */
};

/**
 ** @brief get the data for the next point in time
 **/
brian brian_next(brian);

...
#endif
```

> [!WARNING]
> 每个头文件都必须有 **Include Guard**（包含守卫）。其机制是：第一次 `#include "brian.h"` 时，`BRIAN_H` 未定义，预处理器执行 `#define BRIAN_H 1` 并处理整个文件。第二次包含时 `BRIAN_H` 已定义，`#ifndef` 条件为假，整个文件被跳过。命名约定：头文件名转大写，点号变下划线，如 `brian.h` $\rightarrow$ `BRIAN_H`

## 实现

好程序员写的代码往往注释很少，但仍然可读。代码结构本身展示了"做什么"和"怎么做"，只需要用注释解释不明显的想法

> [!TIP]
> + **字面化地实现**（Implement literally）。C 程序是描述性的文本，命名规则和清晰的控制流是关键
> + **控制流必须是显而易见的**。避免两种模糊控制流的手段
>   + **埋藏跳转**：`break`、`continue`、`return`、`goto` 嵌在复杂结构深处
>   + **蝇头表达式**：`!!++*p--` 或 `a --> 0` 这类写法

### 宏

宏可以是便捷的缩写工具，但使用不当会模糊代码并引发微妙的 bug。宏应该用于初始化常量或实现编译器魔法

> [!WARNING]
> 宏不应该以令人意外的方式改变控制流。以下做法是**错误的**
>
> ```c
> #define begin {
> #define end }
> #define forever for (;;)
> #define ERRORCHECK(CODE) if (CODE) return -1
> ```

#### 悬垂 else 问题

`ERRORCHECK` 宏特别危险——`return` 隐藏在宏中，且会产生悬垂 else 问题

```c
if (a) ERRORCHECK(x);
else puts("a is 0!");
```

宏展开后

```c
if (a) if (x) return -1;
else puts("a is 0!");   // else 绑定到内层 if！
```

等价于

```c
if (a) {
    if (x) return -1;
    else puts("a is 0!");   // 意外！
}
```

#### do-while(false) 技巧

用 `do-while(false)` 包裹宏中的多条语句，是标准手法：用 `{}` 包裹内容，不改变可见的程序结构，避免悬垂 else 问题

```c
#define ERROR_RETURN(CODE)        \
do {                              \
    if (CODE) return -1;          \
} while (false)
```

使用时

```c
if (a) ERROR_RETURN(x);
else puts("a is 0!");
```

展开后 `else` 正确绑定到外层 `if`

```c
if (a) do {
    if (x) return -1;
} while (false);
else puts("a is 0!");   // 正确！
```

> [!TIP]
> 函数式宏在语法上应该表现得像函数调用。可能的陷阱
>
> | 陷阱               | 说明                                                        |
> | :----------------- | :---------------------------------------------------------- |
> | `if` 没有 `else`   | 悬垂 else 问题                                              |
> | 尾随分号           | 可能意外终止外部控制结构                                    |
> | 逗号运算符         | 逗号在大多数场景是列表分隔符，在表达式中是控制运算符。避免使用 |
> | 可绑定表达式       | 替换文本中的参数和表达式要用括号包裹，防止运算符优先级意外   |
> | 多次求值           | 宏是文本替换，参数使用两次则副作用执行两次                   |
>
> 多次求值示例：`#define max(a, b) ((a) < (b) ? (b) : (a))`，调用 `max(i++, 5)` 时 `i` 被 `++` 了两次

### 纯函数

函数参数是**按值传递**的。调用函数时，所有参数先求值，参数（函数的局部变量）接收结果值作为初始化

> [!WARNING]
> 全局变量是不受欢迎的。它们的缺点：代码不灵活（操作的对象是固定的）、难以预测（修改点分散在各处）、难以维护

纯函数的定义：满足以下两个条件

1. 函数除了返回值外，**没有其他副作用**
2. 函数的返回值**只取决于它的参数**

以下副作用会使函数不再是纯函数

+ 读取参数之外的可变程序状态
+ 修改全局对象
+ 在调用之间保持持久的内部状态
+ 执行 IO（如 `printf`）

> [!TIP]
> 尽可能将小任务表达为纯函数。纯函数可以随意移动，甚至可以并行执行。编译器在开启优化后可以原地操作，拷贝开销很小

教材用有理数算术展示了纯函数的威力。所有操作都返回新的 `rat` 值

```c
struct rat {
    bool sign;
    size_t num;
    size_t denom;
};

/* 构造有理数 */
rat rat_get(signed sign, size_t num, size_t denom) {
    rat ret = {
        .sign = (sign < 0),
        .num = num,
        .denom = denom,
    };
    return ret;
}

/* 归一化：约分 */
rat rat_get_normal(rat x) {
    size_t c = gcd(x.num, x.denom);
    x.num /= c;
    x.denom /= c;
    return x;
}

/* 扩展：通分 */
rat rat_get_extended(rat x, size_t f) {
    x.num *= f;
    x.denom *= f;
    return x;
}

/* 乘法 */
rat rat_get_prod(rat x, rat y) {
    rat ret = {
        .sign = (x.sign != y.sign),
        .num = x.num * y.num,
        .denom = x.denom * y.denom,
    };
    return rat_get_normal(ret);
}

/* 加法 */
rat rat_get_sum(rat x, rat y) {
    size_t c = gcd(x.denom, y.denom);
    size_t ax = y.denom / c;
    size_t bx = x.denom / c;
    x = rat_get_extended(x, ax);
    y = rat_get_extended(y, bx);
    assert(x.denom == y.denom);
    if (x.sign == y.sign) {
        x.num += y.num;
    } else if (x.num > y.num) {
        x.num -= y.num;
    } else {
        x.num = y.num - x.num;
        x.sign = !x.sign;
    }
    return rat_get_normal(x);
}
```

> [!NOTE]
> 使用纯函数时，必须把返回值赋给变量（如 `x = rat_get_extended(x, ax)`），否则修改会丢失。C23 提供了 `[[nodiscard]]` 属性，建议编译器在返回值未被使用时发出警告

### 属性

属性（Attributes）是 C23 新引入的特性，用于给代码附加注解信息，帮助编译器和工具进行优化、诊断

C23 标准属性

| 属性             | 含义                                     |
| :--------------- | :--------------------------------------- |
| `deprecated`     | 标记为过时/不建议使用，编译器发出诊断    |
| `fallthrough`    | `switch` 中故意贯穿到下一个 `case`       |
| `maybe_unused`   | 抑制未使用变量/参数的警告                |
| `nodiscard`      | 返回值必须被使用，否则编译器警告         |
| `noreturn`       | 函数永不返回，编译器可优化周围的代码     |
| `unsequenced`    | 纯函数的泛化（无副作用，无持久状态）     |
| `reproducible`   | 可重现（允许读取参数指向的对象，但不修改）|

使用示例

```c
/* deprecated — 标记过时 */
[[deprecated("use new_func instead")]]
void old_func(void);

/* fallthrough — switch 贯穿 */
switch (grade / 10) {
    case 10:
        [[fallthrough]];
    case 9:
        printf("等级: A\n");
        break;
    ...
}

/* maybe_unused — 抑制未使用警告 */
void callback(int data, [[maybe_unused]] void* ctx) {
    // 只用 data，不用 ctx
}

/* nodiscard — 返回值必须使用 */
[[nodiscard]] void* my_malloc(size_t n);

/* noreturn — 函数永不返回 */
[[noreturn]] void fatal_error(const char* msg);
```

#### 前缀属性

除了标准属性，还有带前缀的属性，格式为 `prefix::suffix`

```c
// gcc/clang 的 format 属性，检查 printf 格式字符串
[[__gnu__::__format__(__printf__, 3, 4)]]
int snprintf(char *buf, size_t size, const char *frmt, ...);
```

C23 提供 `__has_c_attribute` 测试特性，用于检查属性是否可用

```c
#if __has_c_attribute(__gnu__::__format__)
  [[__gnu__::__format__(__printf__, 3, 4)]]
#endif
  int snprintf(char *buf, size_t size, const char *frmt, ...);
```

> [!WARNING]
> 属性中的标识符**不是关键字**，可以被预处理器替换，可能和应用层的宏产生灾难性冲突。在头文件中应使用属性的**双下划线形式**
>
> | 标准形式           | 安全形式                 |
> | :----------------- | :----------------------- |
> | `[[fallthrough]]`  | `[[__fallthrough__]]`    |
> | `[[nodiscard]]`    | `[[__nodiscard__]]`      |
> | `[[noreturn]]`     | `[[__noreturn__]]`       |
> | `[[unsequenced]]`  | `[[__unsequenced__]]`    |
> | `[[reproducible]]` | `[[__reproducible__]]`   |
>
> 因为 `__xx__` 形式的标识符是保留的，应用层的宏不会和它们冲突
