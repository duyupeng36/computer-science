# 函数

我们在 [控制](controls.md) 中介绍了 $5$ 中控制语句，它们都是 **有条件的控制转移**。程序根据运行时的数据选择是否指向某些程序块。函数是另一种形式的控制语句，我们称为 **无条件控制语句**；函数调用不需要运行时数据来决定跳转到哪里

> [!TIP]
> 使用函数的 $5$ 大好处
>
> + 避免代码重复（减少 copy-paste 错误）
> + 减少编译时间（同一段代码只编译一次）
> + 方便未来复用
> + 提供清晰的接口（参数类型、返回类型、前后置条件）
> + 天然适合表达使用"栈"的算法（后面讲递归时会体会到）

除了函数，C 还有其他无条件转移控制的手段：`exit/abort`（终止程序）、`goto`（函数内跳转）、`setjmp/longjmp`（跨函数返回）、`signal`（信号处理）。这些目前只需要了解即可。

## 简单函数

我们已经使用过很多函数了，比如 `printf` `strlen` 等。现在，我们学习如何编写自己的函数

在 C 语言中，表示函数的核心语法就是圆括号 `()`。它于表示数组的方括号 `[]` 具有相似的语法地位

+ 声明/定义时：括号里放参数列表（类似数组放维度大小）
+ 调用时：括号里放实参（类似数组 `A[i]` 里放下标）

所有现代 C 函数都必须有 **原型**，即声明中包含完整的**参数类型列表**和**返回类型**。我们已 `leapyear` 函数为例介绍

```c
// 定义（definition）：
bool leapyear(unsigned year) {
    return !(year % 4) && ((year % 100) || !(year % 400));
}

// 声明（declaration，没有函数体）：
bool leapyear(unsigned year);

// 声明时参数名可以省略，还可以加 extern：
extern bool leapyear(unsigned);
```

> [!TIP]
> 函数原型是函数声明或定义中必须包含**完整的参数列表**和**返回类型**；缺少其中任何一个，这样的函数都不存在函数原型。

原型让编译器知道：这个函数接收什么类型、返回什么类型。这样在调用时编译器可以自动做类型转换——比如传 `int 2` 进去，编译器会自动转成 `double 2.0`（如果原型声明参数是 `double`）。

> [!WARNING]
> 请注意：在 C23 版本之前，如果函数不接受参数，则函数声明中的参数列表必须用 `void` 明确表示；否则编译器会假设函数可以接受任意类型和任意数量的参数；这种情况下，没有函数原型。**C23 标准废除了无原型的旧式声明**。换句话说，从 C23 标准开始下面两种声明是等价的
>
> ```c
> int foo(void);
> int foo();
> ```
>
> 虽然，C23 标准废弃了无原型的旧式声明，但是在实际开发中，对于不需要参数的函数必须使用 `void` 明确表示

在函数声明中，`void` 类型有两种特殊用途：**表示函数不接受任何参数** 和 **表示函数不返回值**

```c
// 参数列表写 void：表示函数不接收任何参数
int main(void);

// 返回类型写 void：表示函数不返回值
void swap_double(double* a, double* b);
```

每个函数都必须至少有一个 `return` 语句。唯一例外就是返回类型是 `void` 类型的函数（到达函数体末尾等价于一个不带值的 `return`）；对于非 `void` 类型的函数，`return` 的值必须于函数声明的返回类型一致，或者至少可以隐式转换为返回类型。

> [!TIP]
> + 函数至少有一个 `return` 语句，返回类型是 `void` 类型的函数除外
> + 返回值的类型必须于函数声明的返回类型一致，或至少可以隐式转换为返回类型


下面的两个函数演示函数返回值

```c
// 正确：返回类型和声明一致
int max(int a, int b) {
    if (a > b)
        return a;
    else
        return b;
}  // 两个分支都有 return，没问题

// ❌ 错误：可能没有返回值，但是函数要求返回 int 类型的值
int max(int a, int b) {
    if (a > b)
        return a;
    // 这里如果 a <= b，掉到末尾，没有 return！
}
```

### 示例程序：判定素数

这个示例程序用于演示函数的声明和定义。在 [阶段1-练习#素数判断](phase1-exercise.md#练习-5素数判断15-分钟) 中我们已经遇见过了。这里我们将素数判断的过程提取到一个函数中

```c title="prime3.c" linenums="1"
/* prime3.c - 素数判定函数 */
#include <stdio.h>

// 声明 is_prime 函数：告诉编译器这里有一个函数，接受一个 int 类型的参数，并返回一个 bool 类型的值
bool is_prime(int number);


int main(void) {
    int number = {0};
    printf("请输入一个正整数：");
    scanf("%d", &number);

    if (is_prime(number)) {
        printf("%d 是素数\n", number);
    } else {
        printf("%d 不是素数\n", number);
    }
    return 0;
}

// 定义 is_prime 函数: 告诉编译器函数的具体实现。注意：必须和函数声明保持原型一致
bool is_prime(int number) {
    if (number < 0) {
        number = -number;
    }

    // 2 是已知素数
    if (number == 2) {
        return true;
    }

    // 0、除 2 以外的其他偶数都不是素数
    if ((number == 0) || ((number != 2) && (number % 2 == 0))) {
        return false;
    }

    for (int i = 3; i * i <= number; i += 2) {
        // 只要找到除了 1 和 number 以外的能够整除 number 的因子，number 就不是素数
        if (number % i == 0) {
            return false;
        }
    }

    return true;
}
```

<details>
<summary><strong>NOTE: 编译并运行</strong></summary>

```shell
➜ gcc -Wall -Wextra -std=c23 -o prime3 prime3.c
➜ ./prime3
请输入一个正整数：2
2 是素数

➜ ./prime3
请输入一个正整数：17
17 是素数

➜ ./prime3
请输入一个正整数：100
100 不是素数

➜ ./prime3
请输入一个正整数：-7
-7 是素数

➜ ./prime3
请输入一个正整数：0
0 不是素数
```

</details>

## 特殊的 main 函数

在 [快速入门](getting-started.md) 中我们已经介绍过 `main` 函数是 C 程序在执行环境的入口点。它处于 **运行时系统** 和 **应用程序** 的交界处，因此有一些特殊规则。C 标准只定义了另种 `main` 函数原型

```c
int main(void);                      // 不需要命令行参数
int main(int argc, char* argv[argc+1]);  // 接受命令行参数
```

> [!WARNING]
> 其他形式（如 `void main`、第三参数 `envp`）是平台扩展，不可移植，不要用

头文件 `<stdlib.h>` 中定义两个 `EXIT_*` 宏：`EXIT_SUCCESS` 和 `EXIT_FAILURE`。它们通常定义为

```c
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
```

想要使用这两个名字时，必须 `#include <stdlib.h>`；它们是**唯一**的在所有平台上保证可用的返回值

```c
#include <stdlib.h>

int main(void) {
    // 做一些事...
    if (error) {
        return EXIT_FAILURE;    // 通常是 1
    }
    return EXIT_SUCCESS;        // 通常是 0
}
```

> [!WARNING]
> 注意：`main` 函数执行结束时如果没有 `return` 语句，则默认 `return EXIT_SUCCESS`

### exit() 函数: 提前终止程序

在头文件 `<stdlib.h>` 中，函数 `exit` 用于提取终止程序，其函数原型如下

```c
#include <stdlib.h>

[[noreturn]] void exit(int status);
```

`exit(s)` 等价于在 `main` 中执行 `return s`，但它**可以在任何函数中调用**：

```c
void do_something(void) {
    if (灾难性错误) {
        exit(EXIT_FAILURE);   // 直接终止整个程序
    }
}
```

> [!WARNING]
> 函数 `exit` 永远不会失败，并且永远不会返回。函数原型中的属性 `[[noreturn]]` 标记这个函数不会吧控制权还给调用者

### 命令行参数

当程序可以接收命令行参数时，`main` 函数原型中必须两个参数 `int argc` 和 `char* argv[argc + 1]`

+ `argc` 记录命令行参数的个数
+ `argv` 记录所有命令行参数

请注意：`arc` 中至少为 $1$，表示 `argv` 中最少都有一个参数

```c
int main(int argc, char* argv[argc+1]) {
    // argc: 参数个数
    // argv: 参数字符串数组
}
```

命令行参数列表 `argv` 的内存布局如下

```
argv
├── argv[0] → "./program"       ← 程序名
├── argv[1] → "hello"           ← 第一个参数
├── argv[2] → "42"              ← 第二个参数
│   ...
└── argv[argc] → NULL           ← 末尾是空指针
```

> [!TIP]
> 关于命令行参数，我们有以下几条结论
>
> + 所有命令行参数都是字符串
> + `argv[0]` 指向程序名
> + `argv[argc]` 是空指针（`nullptr`）用于标记 `argv` 结束

由于命令行参数全是字符串，需要自己进行类型转换

```c
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "用法: %s <数字>\n", argv[0]);
        return EXIT_FAILURE;
    }

    double value = strtod(argv[1], nullptr);  // 字符串 → double
    printf("你输入了: %f\n", value);
    return EXIT_SUCCESS;
}
```

## 递归



