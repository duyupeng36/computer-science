# 第 1-2 周练习

> 涵盖：程序基础、控制语句、表达式、基本值和数据、控制流

---

## 一、基础概念题（口头回答）

### 第 1 周

**题目 1**：解释以下 C 表达式的类型和值（假设 `int` 为 32 位）

```c
a. -1U
b. 0x8000'0000
c. sizeof(double)
d. 3 / 2
e. 3.0 / 2
```

答案:

+ a. `-1U` 的类型是 `unsigned int`，值是 `unsigned int` 的最大值 $2^{32}-1$
+ b. `0x8000'0000` 的类型是 `unsigned int`，值是 $2^{31}$
+ c. `sizeof(double)` 的类型 `size_t`，值是 $8$
+ d. `3 / 2` 的类型是 `int`，值是 $1$
+ e. `3.0 / 2` 的类型是 `double`，值是 $1.5$

**题目 2**：以下代码的输出是什么？为什么？

```c
int x = 5;
printf("%d %d %d\n", x++, x, ++x);
```

答案: 未定义行为，输出结果未知。因为，在同一个表达式中出现了多个副作用的子表达式，属于未定义行为

**题目 3**：解释 `const` 和 `constexpr` 的区别。

答案:

+ `const` 标记类型的对象是只读，对象的值在程序运行时才会确定
+ `constexpr` 是 C23 标准引入的关键字，它用于声明编译时常量

### 第 2 周

**题目 4**：以下代码的输出是什么？

```c
int x = 10;
if (x > 5)
    if (x > 15)
        printf("A\n");
else
    printf("B\n");
```

答案: 输出内容是 B

**题目 5**：`switch` 语句中，如果忘记写 `break`，会发生什么？举例说明。

答案: 忘记 break 会有贯穿效应。例如:

```c
int test = 3
switch (test) {
    case 1:
        printf("-");
    case 2:
        printf("--");
    case 3:
        printf("---");
    case 4:
        printf("----");
    case 5:
        printf("-----");
    default:
        printf("======");
}
````

输出结果为

```c
---
----
-----
======
```

**题目 6**：`while` 循环和 `do-while` 循环的核心区别是什么？各举一个适用场景。

答案: 核心区别在于控制表达式的求值时机

+ `while` 语句在执行循环体之前求值
+ `do-while` 语句在执行循环体之后求值

比如，想要计算整数的位数，使用 `do-while`，而不是 `while`。因为，当输入数字是 0 时，也需要进行计算

---

## 二、编程题

### 练习 1：温度转换器（10 分钟）

编写程序，实现摄氏度与华氏度的互相转换。

**要求**：

- 用户输入温度值和单位（C 或 F）
- 输出转换后的温度
- 保留 1 位小数

**公式**：$F = C \times 9/5 + 32$

**示例输出**：

```
请输入温度值和单位（如 36.5 C）：100 C
转换结果：212.0 F
```

答案:

```c title="convert.c"
/* convert.c - 实现摄氏度与华氏度的互相转换。*/
#include <stdio.h>

int main(void) {
    double temperature = {0.0};
    char ch = {0};
    printf("请输入温度值和单位（如 36.5 C）：");
    scanf("%lf", &temperature);
    do {
        ch = getchar();
        if (ch == 'f' || ch == 'F') {
            break;
        }
        if (ch == 'c' || ch == 'C') {
            break;
        }
    } while (true);

    double result = {0};

    switch (ch)
    {
    case 'f':
    case 'F':
        /* 华氏度 -> 摄氏度 */
        /* C = 5(F - 32) / 9 */
        result = 5.0 * (temperature - 32.0) / 9.0;
        printf("转换结果：%.2f C", result);
        break;

    case 'c':
    case 'C':
        /* 摄氏度 -> 华氏度 */
        /* F = C * 9/5 + 32 */
        result = temperature * 9.0 / 5.0 + 32.0;
        printf("转换结果：%.2f F", result);
        break;
    default:
        printf("非法的温度单位: 只允许 F(f) 或 C(c)");
        break;
    }
    return 0;
}
```

---

### 练习 2：打印乘法表（15 分钟）

编写程序，打印 9×9 乘法表。

**要求**：

- 使用嵌套 `for` 循环
- 输出格式对齐

**示例输出**：

```
1×1=1
1×2=2  2×2=4
1×3=3  2×3=6  3×3=9
...
1×9=9  2×9=18 ... 9×9=81
```

答案:

```c title="multiplication-table.c"
/* multiplication-table.c - 打印 9×9 乘法表。*/
#include <stdio.h>

int main(void) {

    for (int i = 1; i < 10; ++i) {
        for (int j = 1; j <= i; ++j) {
            // j < 2 ? 3 : 4 用于指定乘积的最小宽度
            printf("%d * %d = %-*d", j, i,  j < 2 ? 3: 4, j * i);
        }
        printf("\n");
    }
    return 0;
}
```

---

### 练习 3：判断闰年（10 分钟）

编写程序，判断输入的年份是否为闰年。

**规则**：

- 能被 4 整除但不能被 100 整除，或者能被 400 整除

**示例输出**：

```
请输入年份：2024
2024 年是闰年
```

答案:

```c title="leap-year.c"
/* leap-year.c - 判断闰年 */
#include <stdio.h>

int main(void) {
    int year = {0};
    printf("请输入年份：");
    scanf("%d", &year);

    if ((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0))) {
        printf("%d 年是闰年\n", year);
    } else {
        printf("%d 年不是闰年\n", year);
    }
    return 0;
}
```

---

### 练习 4：数字位数统计（15 分钟）

编写程序，统计一个正整数的位数，并输出各位数字之和。

**要求**：

- 使用 `while` 循环
- 不使用字符串

**示例输出**：

```
请输入一个正整数：12345
位数：5
各位数字之和：15
```

答案:

```c title="numdigit2.c"
/* numdigit2.c - 统计整数的位数，并输出各个位上数字之和 */
#include <stdio.h>

int main(void) {
    int number = {0};
    printf("请输入一个正整数：");
    scanf("%d", &number);

    int digits = {0};
    int sum = {0};

    do {
        sum += number % 10;
        number /= 10;
        ++digits;
    } while (number > 0);

    printf("位数：%d\n各位数字之和：%d\n", digits, sum);
    return 0;
}
```

---

### 练习 5：素数判断（15 分钟）

编写程序，判断输入的正整数是否为素数。

**要求**：

- 使用 `for` 循环和 `break`
- 优化：只需检查到 $\sqrt{n}$

**示例输出**：

```
请输入一个正整数：17
17 是素数
```

答案:

```c title="prime.c"
/* prime.c - 判断一个数是否是素数 */
#include <stdio.h>

int main(void) {
    int number = {0};
    printf("请输入一个正整数：");
    scanf("%d", &number);

    if (number <= 0) {
        fprintf(stderr, "请输入正整数，而不是 %d\n", number);
        return 1;
    }

    if (number == 1) {
        printf("%d 不是素数\n", number);
        return 0;
    }

    bool is_prime = true;

    for (int i = 2; i * i <= number; ++i) {
        if (number % i == 0) {
            is_prime = false;
            break;
        }
    }

    if (is_prime) {
        printf("%d 是素数\n", number);
    } else {
        printf("%d 不是素数\n", number);
    }
    return 0;
}
```

---

### 练习 6：猜数字游戏（20 分钟）

编写一个猜数字游戏。

**要求**：

- 程序随机生成 1~100 的数字（可先用固定值测试）
- 用户反复猜测，程序提示"大了"或"小了"
- 猜对后输出次数

**示例输出**：

```
我想了一个 1~100 的数字，请猜：50
大了！再猜：25
小了！再猜：37
大了！再猜：31
恭喜你，猜对了！共猜了 4 次
```

答案:

```c title="guess.c"
/* guess.c - 猜数字 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    time_t t = time(nullptr);
    srand((unsigned int)t);
    int number = rand() % 100 + 1;  // 随机生成一个数字 1 ~ 100 之间的数字
    int guss = {0};
    size_t count = 1;

    printf("我想了一个 1~100 的数字，请猜：");
    scanf("%d", &guss);
    while (guss != number)
    {
        if (guss < number) {
            printf("小了！再猜：");
            scanf("%d", &guss);
        } else if (guss > number)
        {
            printf("大了！再猜：");
            scanf("%d", &guss);
        } else {
            break;
        }
        ++count;
    }

    printf("恭喜你，猜对了！共猜了 %zu 次\n", count);
    return 0;
}
```

---

### 练习 7：位运算练习（15 分钟）

编写程序，实现以下功能：

**要求**：

1. 输入一个无符号整数，输出其二进制表示
2. 统计其中 1 的个数
3. 判断是否是 2 的幂

**示例输出**：

```
请输入一个无符号整数：240
二进制：00000000000000000000000011110000
1 的个数：4
不是 2 的幂
```

**提示**：

- 使用移位运算符 `>>` 和位与 `&`
- 判断 2 的幂：`n & (n - 1) == 0`

答案:

```c title="bitwise.c"
/* bitwise.c - 位运算练习 */
#include <stdio.h>

int main(void) {
    unsigned int number = {0};

    printf("请输入一个无符号整数：");
    scanf("%u", &number);

    // bool 类型：只有 0 或 1 两种取值，用于保存 number 的二进制表示
    bool bits[8 * sizeof(unsigned int)] = {}; // 全部初始化位 0
    bool is2power = !(number & (number - 1));
    size_t ones = 0;

    for (size_t i = 8 * sizeof(unsigned int); i; --i) {
        bits[i-1] = number & 0x1;
        if (bits[i-1]) {
            ++ones;
        }
        number >>= 1;
    }
    printf("二进制：");
    for (size_t i = 0; i < 8 * sizeof(unsigned int); ++i) {
        printf("%d", bits[i]);
    }
    printf("\n1 的个数：%zu", ones);
    if (is2power) {
        printf("\n是 2 的幂\n");
    } else {
        printf("\n不是 2 的幂\n");
    }
    return 0;
}
```

---

### 练习 8：类型转换陷阱（10 分钟）

预测以下程序的输出，然后运行验证：

```c
#include <stdio.h>

int main(void) {
    int a = -1;
    unsigned int b = 0;

    printf("a < 0: %d\n", a < 0);
    printf("b == 0: %d\n", b == 0);
    printf("a < b: %d\n", a < b);

    signed int c = 0x8000'0000;
    printf("c = %d\n", c);

    return 0;
}
```

**问题**：

1. 为什么 `a < b` 的结果不是你预期的？
2. `c` 的值为什么是负数？

答案:

输出结果

+ printf("a < 0: %d\n", a < 0); ==> a < 0: 1
+ printf("b == 0: %d\n", b == 0); ==> b == 0: 1
+ printf("a < b: %d\n", a < b); ==> a < b: 0
+ printf("c = %d\n", c); ==> c = -2147483648

问题答案:

+ 为什么 `a < b` 的结果不是你预期的？因为 `a` 的类型是 int 而 b 的类型是 unsigned int。在表达式中，如果混用了有符号类型和无符号类型，则有符号值转换为无符号值。由于 -1 的无符号值是 UINT_MAX，这是一个非常大的数
+ `c` 的值为什么是负数？这是取决于具体实现，我使用的具体实现会重用位模式， 0x8000'0000 作为补码解释时，值为 -2147483648

---

### 练习 9：成绩等级判定（10 分钟）

编写程序，根据输入的分数输出等级。

**规则**：

- 90~100：A
- 80~89：B
- 70~79：C
- 60~69：D
- 0~59：F
- 其他输入：提示非法

**要求**：使用 `switch` 语句（提示：`score / 10`）

**示例输出**：

```
请输入成绩：85
等级：B
```

答案:

```c title="grade.c"
/* grade.c - 成绩等级 */
#include <stdio.h>

int main(void) {
    int grade = {0};
    printf("请输入成绩：");
    scanf("%d", &grade);

    switch (grade / 10)
    {
    case 10:
        [[fallthrough]];
    case 9:
        printf("等级: A\n");
        break;
    case 8:
        printf("等级: B\n");
        break;
    case 7:
        printf("等级: C\n");
        break;
    case 6:
        printf("等级: D\n");
        break;

    default:
        printf("等级: F\n");
        break;
    }
}
```

---

### 练习 10：打印菱形（15 分钟）

输入一个奇数 $n$，打印 $n$ 行的菱形。

**要求**：

- 使用嵌套 `for` 循环
- 先打印上半部分（含中间行），再打印下半部分

**示例输出**（n=5）：

```
    *
   ***
  *****
   ***
    *
```

答案:

```c title="rhombus.c"
/* rhombus.c - 打印菱形 */
#include <stdio.h>

int main(void) {

    int layer = {0};

    printf("输入菱形的层级(奇数)：");
    scanf("%d", &layer);
    if (!(layer & 0x1)) {
        fprintf(stderr, "请输入奇数，而不是偶数 %d\n", layer);
        return 1;
    }

    // 打印上半部分
    for (int i = 1; i <= layer / 2; ++i) {
        for (int j = 0; j < 2 * i - 1; ++j) {
            if (j == 0) {
                printf("%*c", layer - i, '*');
                continue;
            }
            printf("*");
        }
        printf("\n");
    }
    // 打印下半部分
    for (int i = layer / 2 + 1; i; --i) {
        for (int j = 0; j < 2 * i - 1; ++j) {
            if (j == 0) {
                printf("%*c", layer - i, '*');
                continue;
            }
            printf("*");
        }
        printf("\n");
    }

    return 0;
}
```

---

### 练习 11：最大公约数（10 分钟）

编写程序，输入两个正整数，求它们的最大公约数（GCD）。

**要求**：

- 使用辗转相除法（Euclid 算法）
- 使用 `while` 循环

**示例输出**：

```
请输入两个正整数：48 36
最大公约数：12
```

```c title="gcd.c"
/* gcd.c - 计算两个数的最大公约数 */
#include <stdio.h>

int main(void) {
    int a = {0};
    int b = {0};
    printf("请输入两个正整数: ");
    scanf("%d%d", &a, &b);

    if (a < b) {
        int temp = a;
        a = b;
        b = temp;
    }

    while (b != 0)
    {
        int r = a % b;
        if (r == 0) {
            break;
        }
        a = b;
        b = r;
    }

    printf("最大公约数是: %d\n", b);
    return 0;
}
```

---

### 练习 12：猜数字游戏改进版（20 分钟）

在练习 6 猜数字游戏的基础上改进：

**要求**：

- 程序随机生成 1~100 的数字
- 用户最多猜 7 次
- 每次猜测后提示"大了"或"小了"
- 猜对输出恭喜，7 次用完输出答案
- 使用 `for` 循环 + `break`

**示例输出**：

```
我想了一个 1~100 的数字，你有 7 次机会！
第 1 次猜测：50
大了！
第 2 次猜测：25
小了！
第 3 次猜测：37
恭喜你，猜对了！共猜了 3 次
```

答案

```c title="guess2.c"
/* guess2.c - 猜数字改进版：用户最多猜测 7 次  */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(void) {
    time_t t = time(nullptr);
    srand((unsigned int)t);
    int number = rand() % 100 + 1;  // 随机生成一个数字 1 ~ 100 之间的数字
    int guess = {0};

    constexpr size_t count = 7;

    printf("我想了一个 1~100 的数字，你有 7 次机会！\n");
    bool guessed = false;
    for (size_t i = 0; i < count; ++i) {
        printf("第 %zu 次猜测：", i + 1);
        scanf("%d", &guess);
        if (guess > number) {
            printf("大了\n");
        } else if (guess < number) {
            printf("小了\n");
        } else {
            guessed = true;
            printf("恭喜你，猜对了！共猜了 %zu 次", i + 1);
            break;
        }
    }
    if (!guessed) {
        printf("抱歉，猜错了！正确值是 %d\n", number);
    }
    return 0;
}
```

---

### 练习 13：打印质数（15 分钟）

编写程序，输出 2~100 之间的所有质数。

**要求**：

- 使用嵌套循环（外层遍历数字，内层判断是否为质数）
- 使用 `break` 优化内层循环
- 每行输出 10 个质数

**示例输出**：

```
2 3 5 7 11 13 17 19 23 29
31 37 41 43 47 53 59 61 67 71
73 79 83 89 97
```

答案:

```c title="prime2.c"
/* prime2.c - 输出 2 ~ 100 中的所有素数 */
#include <stdio.h>

int main(void) {

    printf("%3d", 2);
    int count = 1;
    for (int number = 3; number <= 100; number += 2) {
        bool flag = true;
        for (int factor = 2; factor * factor <= number; ++factor) {
            if (number % factor == 0) {
                flag = false;
                break;
            }
        }
        if (flag) {
            printf("%3d", number);
            ++count;
        }
        if (count % 10 == 0) {
            printf("\n");
        }
    }
    return 0;
}
```

---

### 练习 14：数字金字塔（15 分钟）

输入一个正整数 $n$，打印如下数字金字塔。

**要求**：

- 使用嵌套 `for` 循环
- 数字之间用空格分隔，整体居中对齐

**示例输出**（n=5）：

```
    1
   2 2
  3 3 3
 4 4 4 4
5 5 5 5 5
```

答案:

```c title="digital-pyramid.c"
/* digital-pyramid.c - 数字金字塔 */
#include <stdio.h>

int main(void) {

    int layer = {0};
    printf("输入一个正整数: ");
    scanf("%d", &layer);
    if (layer < 0) {
        fprintf(stderr, "请输入正整数");
        return 1;
    }

    for (int i = 1; i <= layer; ++i) {
        for (int j = 0; j < i; j++) {
            if (j == 0) {
                printf("%*d", layer - i + 1, i);
                continue;
            }
            printf(" %d", i);
        }
        printf("\n");
    }

    return 0;
}
```

---

### 练习 15：Fibonacci 数列（10 分钟）

编写程序，输出 Fibonacci 数列的前 20 项。

**要求**：

- 使用 `for` 循环
- Fibonacci 数列：1, 1, 2, 3, 5, 8, 13, 21, ...
- 每行输出 5 个数

**示例输出**：

```
1 1 2 3 5
8 13 21 34 55
89 144 233 377 610
987 1597 2584 4181 6765
```

答案:

```c title="fibonacci.c"
/* fibonacci.c - 斐波拉契数列 */
#include <stdio.h>

int main(void) {

    constexpr int count = 20;

    for (int i = 1, a = 1, b = 1; i <= count; ++i) {
        printf("%d ", a);
        int temp = a;
        a = b;
        b = temp + a;

        if (i % 5 == 0) {
            printf("\n");
        }
    }
    return 0;
}
```
---

## 三、思考题

### 第 1 周

**题目 1**：以下代码有什么问题？如何修复？

```c
#define ARRAY_SIZE 10

int main(void) {
    int arr[ARRAY_SIZE];
    for (int i = 0; i <= ARRAY_SIZE; i++) {
        arr[i] = i * 2;
    }
    return 0;
}
```

答案

+ for 循环中的控制表达式 `i <= ARRAY_SIZE` 会导致数组越界(数组的索引是从 0 开始到数组长度 - 1)。应该修改为 `i < ARRAY_SIZE`

**题目 2**：为什么以下代码可能输出意外结果？

```c
double x = 0.1 + 0.2;
if (x == 0.3) {
    printf("相等\n");
} else {
    printf("不相等：%f\n", x);
}
```

答案

+ "不相等：0.3xxxxx"。原因在于 十进制小数无法精确的表示成二进制形式，并且 double 类型有固定的长度，无法完全表示小数

**题目 3**：解释以下代码中 `const` 的作用：

```c
char const *const str = "hello";
```

答案

+ 确保 str 指针不能修改，同时还确保不能通过指针修改对象

### 第 2 周

**题目 4**：以下代码有什么问题？如何修复？

```c
int i = 0;
while (i < 10) {
    if (i == 5) {
        i++;
        continue;
    }
    printf("%d ", i);
    i++;
}
```

答案: 当 i = 5 时，循环会跳过 `continue` 后面的所有代码开始下一次循环。由于跳过了 `i++` 导致 `i` 没有发生变化，从而进入死循环。修改方法可以在 `continue` 前面补充一个 `i++`

**题目 5**：以下代码的输出是什么？为什么？

```c
for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
        if (j == 3) {
            break;
        }
        printf("(%d,%d) ", i, j);
    }
}
```

`break` 跳出的是哪一层循环？

答案

+ 输出结果: `(0, 0) (0,1) (0,2) (1,0) (1,1) (1,2) (2,0) (2,1) (2,2) (3,0) (3,1) (3,2) (4,0) (4,1) (4,2)`
+ `break` 退出的时内层循环(循环变量为 `j` 的循环)

**题目 6**：以下两种写法有什么区别？哪种更好？

```c
// 写法 A
for (int i = 0; i < n; i++) {
    // ...
}

// 写法 B
int i = 0;
while (i < n) {
    // ...
    i++;
}
```

答案: 写法A 和 写法B 在逻辑上是等价的。但是，写法A 的循环变量在离开 `for` 语句后就不可见了；然而，写法B 的循环遍历在离开 `while` 语句后仍然可以访问。至于哪种写法更好取决于需求；如果后续代码不需要循环遍历，使用写法A 更好；否则，使用写法B更好

---

## 四、挑战题（选做）

### 挑战 1：打印菱形

输入一个奇数 $n$，打印 $n$ 行的菱形。

**示例**（n=5）：

```
    *
   ***
  *****
   ***
    *
```

### 挑战 2：哥德巴赫猜想验证

验证 4~100 之间的所有偶数都可以表示为两个素数之和。

**输出格式**：

```
4 = 2 + 2
6 = 3 + 3
8 = 3 + 5
...
```

### 挑战 3：自制 `strlen`

不使用 `<string.h>`，实现自己的 `strlen` 函数。

### 挑战 4：打印日历

输入年份和月份，打印该月的日历。

**要求**：

- 判断闰年（第 1 周已有代码）
- 计算该月 1 号是星期几（可使用蔡勒公式或基姆拉尔森公式）
- 格式对齐，星期日到星期六排一行

**示例输出**（2026 年 7 月）：

```
日 一 二 三 四 五 六
          1  2  3  4
 5  6  7  8  9 10 11
12 13 14 15 16 17 18
19 20 21 22 23 24 25
26 27 28 29 30 31
```

### 挑战 5：Collatz 猜想

输入一个正整数 $n$，输出从 $n$ 到 1 的 Collatz 序列。

**规则**：

- 如果 $n$ 是偶数，$n = n / 2$
- 如果 $n$ 是奇数，$n = 3n + 1$
- 重复直到 $n = 1$

**要求**：

- 使用 `do-while` 循环
- 统计序列长度
- 输出序列和长度

**示例输出**：

```
请输入一个正整数：6
序列：6 3 10 5 16 8 4 2 1
长度：9
```

### 挑战 6：自制 `strchr`

不使用 `<string.h>`，实现自己的 `strchr` 函数。

**要求**：

- 函数原型：`const char *my_strchr(const char *s, int c)`
- 在字符串 `s` 中查找字符 `c` 第一次出现的位置
- 找到返回指向该字符的指针，未找到返回 `NULL`
- 用 `main` 函数测试

---

## 练习建议

| 优先级 | 练习 | 预计时间 |
| --- | --- | --- |
| ⭐⭐⭐ | 练习 1-13 | 135 分钟 |
| ⭐⭐ | 练习 14-16 | 40 分钟 |
| ⭐ | 思考题 | 40 分钟 |
| 选做 | 挑战题 | 各 15 分钟 |

**建议顺序**：先做编程题（1-13），再做练习 14-16，最后做思考题。挑战题有余力再做。
