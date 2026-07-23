# 快速入门

C 程序的目的是让计算机完成特定任务。C 程序通过"下命令"来实现，就像人类语言中的祈使句一样，因此这种编程方式叫做**命令式编程**（imperative programming）

> [!TIP]
> C 是一种命令式编程语言

教材开篇引用了《银河系漫游指南》：**Don't panic。** 无论你有编程经验还是第一次接触，都要有耐心

## 第一个 C 程序

```c
/* This may look like nonsense, but really is -*- mode: C -*- */
#include <stdlib.h>
#include <stdio.h>

/* The main thing that this program does. */
int main(int argc, [[maybe_unused]] char* argv[argc+1]) {
    // Declarations
    double A[5] = {
        [0] = 9.0,
        [1] = 2.9,
        [4] = 3.E+25,
        [3] = .00007,
    };

    // Doing some work
    for (size_t i = 0; i < 5; ++i) {
        printf("element %zu is %g, \tits square is %g\n",
               i,
               A[i],
               A[i]*A[i]);
    }

    return EXIT_SUCCESS;
}
```

运行输出

```
element 0 is 9,           its square is 81
element 1 is 2.9,         its square is 8.41
element 2 is 0,           its square is 0
element 3 is 7e-05,       its square is 4.9e-09
element 4 is 3e+25,       its square is 9e+50
```

程序的主要目的是打印 $5$ 行文本。核心动作是 `printf` 调用。C 把这种调用叫做**语句**（statement），其他语言可能叫"指令"（instruction）

`printf` 函数接收 $4$ 个**参数**（arguments），用括号包围

+ 第 $1$ 个参数是**字符串字面量**（string literal），用作**格式**（format）。其中有 $3$ 个**格式说明符**（format specifiers）：`%zu`、`%g`、`%g`，用 `%` 开头。格式中还有**转义字符**（escape characters）：`\t`（制表符）和 `\n`（换行符）
+ 第 $2$ 个参数 `i`，对应第一个 `%zu`
+ 第 $3$ 个参数 `A[i]`，对应第一个 `%g`
+ 第 $4$ 个参数 `A[i]*A[i]`，对应第二个 `%g`

> 现在不必理解所有参数的含义，只需要知道程序的主要目的是打印 $5$ 行文本，它"命令" `printf` 函数来完成这个任务。其余部分只是指定打印哪些数字、打印多少个

## 编译与运行

程序文本只是我们写的一段文字，计算机本身无法理解它。需要一个特殊的程序——**编译器**（compiler）——把 C 文本翻译成计算机能理解的**二进制代码**（binary code）或**可执行文件**（executable）

> [!TIP]
> C 是一种编译型编程语言

编译器的名字和命令行参数取决于**平台**（platform）。目标二进制代码是**平台相关**的：PC、手机、冰箱的需求各不相同。C 的存在理由之一就是为所有不同的机器语言（**汇编语言**，assembly language）提供一个抽象层

> [!TIP]
> 正确的 C 程序可以在不同平台之间移植

编译命令

```shell
c17 -Wall -o getting-started getting-started.c -lm
```

+ `c17` — 编译器程序
+ `-Wall` — 告诉编译器对任何异常发出警告
+ `-o getting-started` — 输出文件名为 `getting-started`
+ `getting-started.c` — 源文件（`.c` 扩展名表示 C 语言）
+ `-lm` — 链接标准数学函数库

如果 `c17` 不可用，可以尝试其他编译器

```shell
clang -std=c2x -Wall -lm -o getting-started getting-started.c
gcc -std=c2x -Wall -lm -o getting-started getting-started.c
icc -std=c2x -Wall -lm -o getting-started getting-started.c
```

其中 `-std=c2x` 指定 C23 标准（C2x 是 C23 发布前的名称）

编译成功后，运行可执行文件

```shell
./getting-started
```

这就是"可移植"的含义：无论在哪里运行，程序的行为应该相同

### 有缺陷的程序

教材展示了一个有缺陷的程序（bad.c），与 getting-started.c 只有两处关键差异

```c
/* 问题程序 */
void main() {                    // ← 错误1：main 返回类型应为 int
    int i;
    double A[5] = { 9.0, 2.9, 3.E+25, .00007 };
    for (i = 0; i < 5; ++i) {
        printf("element %d is %g, \tits square is %g\n",  // ← 错误2：缺少 #include <stdio.h>
               i, A[i], A[i]*A[i]);
    }
    return 0;                    // ← 错误3：void 函数不应有返回值
}
```

GCC 编译时输出了 $3$ 条警告

1. `main` 的返回类型不应该是 `void`，应该是 `int`
2. `printf` 是隐式声明——缺少 `#include <stdio.h>`
3. `void` 函数中 `return 0` 不正确

GCC 只输出警告，仍然生成了可执行文件。但 Clang 更严格，直接报错拒绝编译。可以用 `-Werror` 选项让 GCC 也把警告当作错误处理

> [!WARNING]
> C 程序应该干净地编译，没有警告

两个程序之间还有**第三处差异**：数组 `A` 的初始化方式不同。getting-started.c 使用了指定初始化，`A[2]` 被隐式初始化为 $0$；bad.c 使用顺序初始化，只有 $4$ 个值，`A[4]` 被隐式初始化为 $0$，`A[2]` 反而得到了 `3.E+25`。两者的 `A[2]` 和 `A[4]` 值互换了

## 程序的语法

C 程序由以下几类文本元素组成

### 特殊词

getting-started.c 中用到了这些特殊词：`#include`、`int`、`maybe_unused`、`char`、`void`、`double`、`for`、`return`。它们代表 C 语言预定义的概念和功能，不能更改。在 C 术语中，它们分别叫做**指令**（directives）、**关键字**（keywords）、**属性**（attributes）和**保留标识符**（reserved identifiers）

### 标点符号

C 使用多种标点符号来组织程序文本

+ $6$ 种括号：`{...}`、`(...)`、`[...]`、`[[...]]`、`/*...*/`、`<...>`。括号总是成对出现。`<...>` 很少使用，只在同一行内。其他 $5$ 种可以跨多行
+ $2$ 种分隔符/终止符：逗号 `,` 和分号 `;`

> [!TIP]
> 标点符号可以用于多种不同的目的。例如 `{}` 和 `[]` 在 getting-started.c 中各有 $3$ 种不同用途

### 注释

```c
/* 这是传统 C 风格注释，可以跨多行 */

// 这是 C++ 风格注释，只到行尾
```

注释被编译器忽略，是解释和文档化代码的最佳位置

### 字面量

程序中直接出现的固定值：`0`、`1`、`3`、`4`、`5`、`9.0`、`2.9`、`3.E+25`、`.00007`、`"element %zu is %g, \tits square is %g\n"`

### 标识符

标识符是"名字"。getting-started.c 中有：`A`、`i`、`main`、`printf`、`size_t`、`EXIT_SUCCESS`。标识符可以扮演不同角色

+ **数据对象**（data objects），也叫**变量**（variables）：如 `A` 和 `i`
+ **类型别名**（type aliases）：如 `size_t`。尾部的 `_t` 后缀是 C 标准用来提醒你这是一个类型的命名约定
+ **函数**（functions）：如 `main` 和 `printf`
+ **常量**（constants）：如 `EXIT_SUCCESS`

### 函数

`main` 和 `printf` 是两个函数标识符。`printf` 用于产生输出。`main` 被**定义**：它的声明后面跟着**函数体**（function body）`{...}`，描述了这个函数要做什么。`main` 在 C 程序中有特殊角色——它必须始终存在，因为它是程序执行的入口点

### 运算符

程序中用到了几个运算符

+ `=` 用于**初始化**和**赋值**
+ `<` 用于比较
+ `++` 用于自增（将变量的值加 $1$）
+ `*` 用于乘法

### 属性（C23）

`[[maybe_unused]]` 这样的属性放在双方括号中，为程序提供补充信息。这是 C23 的新特性

> [!TIP]
> 正如自然语言一样，C 程序的词法元素和语法要与其传达的实际含义区分开来。与自然语言不同的是，C 的含义是严格规定的，通常没有歧义空间

## 声明

在程序中使用某个标识符之前，必须先给编译器一个**声明**（declaration），说明这个标识符代表什么。关键字是语言预定义的，不需要也不能被声明或重定义

> [!TIP]
> 程序中所有标识符都必须被声明

getting-started.c 中有 $5$ 个在程序中实际声明的标识符

```c
int main(int, char*[]);
int argc;
[[maybe_unused]] char* argv[];
double A[5];
size_t i;
```

每个声明都有一个标识符和关联的属性

+ `i` 的类型是 `size_t`
+ `argc` 的类型是 `int`
+ `main` 后面跟着括号 `(...)`，声明了一个返回 `int` 的函数
+ `A` 后面跟着方括号 `[...]`，声明了一个**数组**（array）——$5$ 个 `double` 类型元素的有序集合，通过**索引**（indices）从 $0$ 到 $4$ 引用
+ `argv` 也跟着方括号，也是数组。属性 `[[maybe_unused]]` 表示可能未使用

变量可视化为"盒子"

```
  argc       int ??

    i        size_t ??

             [0]          [1]           [2]           [3]          [4]
  A        double ??    double ??    double ??    double ??    double ??
```

概念上区分 $4$ 个东西

+ **盒子本身** $\rightarrow$ 对象（object）
+ **规格说明** $\rightarrow$ 类型（type）
+ **盒子内容** $\rightarrow$ 值（value）
+ **写在盒子上的名字** $\rightarrow$ 标识符（identifier）

`??` 表示还不知道实际值

### 预声明的标识符

另外 $3$ 个标识符 `printf`、`size_t`、`EXIT_SUCCESS` 在程序中没有看到声明。它们是**预声明**的。通过头文件告诉编译器去哪里找

```c
#include <stdio.h>    // 提供 printf
#include <stdlib.h>   // 提供 size_t, EXIT_SUCCESS
```

这些标识符的真实声明可能是这样的

```c
int printf(char const format[static 1], ...);
typedef unsigned long size_t;
#define EXIT_SUCCESS 0
```

因为这些预声明的具体细节通常不太重要，它们被隐藏在**头文件**（header files）中。如果需要查看语义，不要直接去看头文件（那些文件几乎不可读），应该查平台文档

```shell
apropos printf
man printf
man 3 printf
```

声明只是描述一个特性，不会创建它，所以重复声明不会造成太多伤害。标识符可以有多个一致的声明

> [!TIP]
> 声明绑定到它们出现的**作用域**（scope）——标识符**可见**的那部分程序

getting-started.c 中有不同作用域的声明

+ `A` 在 `main` 函数定义内部可见，从第 $8$ 行到第 $24$ 行
+ `i` 绑定到 `for` 结构中，从第 $16$ 行到第 $21$ 行
+ `main` 从声明开始到文件末尾都可见
+ `argc` 和 `argv` 是函数的**参数**（parameters），作用域跨越整个函数体

前两种叫做**块作用域**（block scope）。块是语法中封装声明的结构。`main` 函数连同参数列表和函数体构成一个块。`for` 构造形成一个**主块**（primary block），循环体形成一个**次块**（secondary block）。块可以**嵌套**

`main` 本身不在任何 `(...)` 或 `{...}` 对内部的作用域叫做**文件作用域**（file scope）。文件作用域中的标识符通常称为**全局**（globals）

所以，这个看似简单的程序有 $4$ 层嵌套作用域：文件作用域和 $3$ 层嵌套块

## 定义

声明只说明标识符的种类，不指定具体值，也不指定对象在哪里。这个重要角色由**定义**（definition）来承担

> [!TIP]
> 声明指定标识符，而定义指定对象

**初始化**（initialization）是附加在声明上的语法构造，为对象提供初始值。带初始化器的声明同时也定义了对象

```c
size_t i = 0;
```

盒子可视化：`i` 的值为 $0$

`A` 的初始化使用了**指定初始化**（designated initializer）：方括号中的数字指定数组的哪个元素被初始化

```c
double A[5] = {
    [0] = 9.0,
    [1] = 2.9,
    [4] = 3.E+25,
    [3] = .00007,
};
```

这将 $5$ 个元素初始化为 `9.0`、`2.9`、`0.0`、`0.00007`、`3.0E+25`

```
             [0]            [1]            [2]              [3]                [4]
A        double 9.0      double 2.9    double 0.0     double 0.00007     double 3.0E+25
```

初始化器中未列出的位置默认设为 $0$。缺失的 `[2]` 被填充为 `0.0`

> [!TIP]
> 数组索引从 $0$ 开始。把索引想象成对应元素到数组起始位置的**距离**。对于有 $n$ 个元素的数组，第一个元素索引为 $0$，最后一个为 $n-1$

对于函数，如果声明后面跟着 `{...}` 包含函数代码，就是定义

```c
int main(int argc, [[maybe_unused]] char* argv[argc+1]) {
    ...
}
```

与声明（允许同一个标识符有多个）不同，定义必须是唯一的。每个对象或函数必须恰好有一个定义

## 语句

语句是告诉编译器如何处理已声明标识符的指令。`main` 函数的第二部分主要是语句

```c
for (size_t i = 0; i < 5; ++i) {
    printf("element %zu is %g, \tits square is %g\n",
           i,
           A[i],
           A[i]*A[i]);
}

return EXIT_SUCCESS;
```

教材将语句分为三类

### 迭代

`for` 语句告诉编译器：程序应该执行循环体若干次。这是 C 中最简单的**域迭代**（domain iteration）形式。它有 $4$ 个部分

```c
for (size_t i = 0; i < 5; ++i) {
    // 循环体（loop body）
}
```

`(...)` 内由分号分成 $3$ 部分

1. **初始化**：`size_t i = 0` — 循环变量的声明和初始化。在整个 `for` 语句执行前只执行一次
2. **循环条件**：`i < 5` — 指定迭代何时继续。每次执行循环体之前检查。只要 `i` 严格小于 $5$ 就继续
3. **迭代表达式**：`++i` — 每次迭代之后执行。将 `i` 的值加 $1$

合起来，程序执行循环体 $5$ 次，`i` 的值依次为 $0$、$1$、$2$、$3$、$4$

> [!TIP]
> + 域迭代应该用 `for` 语句编码
> + 循环变量应该在 `for` 的初始部分定义，不要在外部定义或复用。这样读者和编译器都知道这个变量对那个特定 `for` 循环有特殊的迭代计数器含义

### 函数调用

函数调用是特殊的语句，它**暂停**当前函数的执行，然后把控制权交给被调用的函数

```c
printf("element %zu is %g, \tits square is %g\n",
       i,
       A[i],
       A[i]*A[i]);
```

被调用的函数是 `printf`。函数调用通常不只是提供函数名，还提供参数。这些参数的**值**被传递给函数

> [!TIP]
> 关键词是**值**：虽然 `i` 是参数，但 `printf` 永远无法改变 `i` 本身。这种机制叫做**按值传递**（call by value）。C 没有按引用传递；通过取地址和传递指针来实现类似效果，后面会学到

### 函数返回

`main` 中最后一个语句是 `return`。它告诉 `main` 函数完成后返回到调用它的地方。因为 `main` 的声明中有 `int`，所以 `return` 必须送回一个 `int` 类型的值。这里返回 `EXIT_SUCCESS`

虽然看不到 `printf` 的定义，但它内部也必须有类似的 `return` 语句。在调用 `printf` 时，`main` 的语句执行被暂停。`printf` 执行直到遇到 `return`，然后控制权回到 `main`，从暂停处继续执行

```
平台启动程序 → 调用 main() → 调用 printf() → printf 返回 → main 返回 → 启动程序
```

左边是平台提供的进程启动例程，中间是程序员写的 `main`，右边是 C 库中的 `printf`。遇到 `return` 后控制权逐层返回。从程序员的角度看，`main` 返回启动程序就是程序执行的结束

---

## 练习

### 基础概念题（口头回答）

**题目 1**【快速入门】：解释 C 程序从源代码到可执行文件的编译流程。编译器的作用是什么？

**题目 2**【快速入门】：`getting-started.c` 和 `bad.c` 有哪三处关键差异？每处差异会导致什么问题？

**题目 3**【快速入门】：以下程序有什么问题？

```c
void main() {
    printf("hello\n");
    return 0;
}
```

**题目 4**【快速入门】：解释以下概念的区别
+ 声明（declaration）和定义（definition）
+ 对象（object）和值（value）
+ 标识符（identifier）和关键字（keyword）

**题目 5**【快速入门】：以下代码中，变量 `x` 的作用域是什么？`i` 的作用域是什么？

```c
int x = 10;

int main(void) {
    for (int i = 0; i < 5; ++i) {
        printf("%d\n", x + i);
    }
    return 0;
}
```

**题目 6**【快速入门】：以下数组 `A` 的各元素值是多少？

```c
double A[5] = {
    [1] = 3.14,
    [3] = 2.72,
};
```

**题目 7**【快速入门】：`for` 循环的三个部分分别是什么？各执行几次？

```c
for (int i = 0; i < 10; ++i) {
    printf("%d\n", i);
}
```

**题目 8**【快速入门】：以下程序的输出是什么？为什么？

```c
#include <stdio.h>

int main(void) {
    int x = 5;
    printf("x = %d\n", x);
    // printf("x = %d\n", x++);
    return 0;
}
```

如果取消注释第二行 `printf`，会发生什么？

**题目 9**【快速入门】：以下代码有多少个记号（token）？分别是什么类型？

```c
int result = a + b * 2;
```

**题目 10**【快速入门】：以下程序的输出是什么？

```c
#include <stdio.h>

int main(void) {
    int a[3] = {[0] = 1, [2] = 3};
    printf("%d %d %d\n", a[0], a[1], a[2]);
    return 0;
}
```

### 编程题

#### 练习 1：修复 bad.c（10 分钟）【快速入门】

将 `bad.c`（code 目录下）逐步修复为正确的程序。要求

+ 从第一个诊断信息开始，修复代码，重新编译
+ 重复直到程序干净编译，没有警告
+ 编译命令：`gcc -Wall -Werror -std=c23 -o bad_fixed bad.c`

---

#### 练习 2：指定初始化练习（10 分钟）【快速入门】

编写程序，声明一个 `int` 类型的数组 `data[10]`，使用指定初始化

+ 将 `[0]` 初始化为 $100$
+ 将 `[5]` 初始化为 $500$
+ 将 `[9]` 初始化为 $900$
+ 其余元素默认为 $0$
+ 使用 `for` 循环打印所有元素的值和索引

**示例输出**：

```
data[0] = 100
data[1] = 0
...
data[5] = 500
...
data[9] = 900
```

---

#### 练习 3：printf 格式练习（15 分钟）【快速入门】

编写程序，声明 `int`、`double`、`size_t` 三种类型的变量，使用不同的格式说明符输出

要求

+ 使用 `%d` 打印 `int`
+ 使用 `%f`、`%e`、`%g` 三种格式打印同一个 `double` 值
+ 使用 `%zu` 打印 `size_t`
+ 使用 `%o` 和 `%x` 打印同一个 `int` 值（八进制和十六进制）

**示例输出**：

```
int: 42
double (f): 3.141590
double (e): 3.141590e+00
double (g): 3.14159
size_t: 100
octal: 52
hex: 2a
```

---

#### 练习 4：作用域观察（10 分钟）【快速入门】

以下程序的输出是什么？先预测，再运行验证

```c
#include <stdio.h>

int x = 100;

int main(void) {
    int x = 200;
    printf("外层 x = %d\n", x);

    for (int x = 0; x < 3; ++x) {
        printf("循环 x = %d\n", x);
    }

    printf("外层 x = %d\n", x);
    return 0;
}
```

问题

+ 程序中有几个 `x`？它们各自的作用域是什么？
+ `for` 循环结束后，外层的 `x` 值变了吗？为什么？

---

### 思考题

**思考题 1**【快速入门】：教材说"C 是一种编译型语言"。你知道有哪些语言是编译型的，哪些是解释型的？编译型和解释型各有什么优缺点？

**思考题 2**【快速入门】：教材说"正确的 C 程序可以在不同平台之间移植"。但现实中，有些 C 程序在不同平台上行为不同。你能想到哪些原因？

**思考题 3**【快速入门】：为什么 C 语言的数组索引从 $0$ 开始而不是从 $1$ 开始？（提示：教材说"把索引想象成对应元素到数组起始位置的距离"）
