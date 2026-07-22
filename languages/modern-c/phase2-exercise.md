# 阶段2-练习

> 涵盖：快速入门、控制语句、表达式、基本类型、派生类型、函数、库函数

---

## 一、基础概念题（口头回答）


**题目 1**：解释 C 程序从源代码到可执行文件的编译流程。

答案:

+ **预处理阶段**: 执行所有的预处理指令，并删除代码中的注释
+ **编译阶段**: 将预处理器的输出编译为汇编代码
+ **汇编阶段**: 将汇编代码转换为目标文件。到此，就完成的文本到二进制文件的转变
+ **链接**: 链接程序使用的库函数、启动代码等，从而生成可执行文件


**题目 2**：以下代码的输出是什么？为什么？

```c
int x = 10;
int y = x++ + ++x;
printf("y = %d\n", y);
```

答案: 同一个表达式中出现两个及以上的副作用属于未定义行为。输出结果不可知


**题目 3**：`struct` 和 `typedef struct` 的区别是什么？

答案：`struct` 是定义结构类型，而 `typedef struct` 只是单纯的给 `struct` 类型取一个别名

**题目 4**：以下函数声明中 `static 1` 是什么意思？

```c
void puts(char const s[static 1]);
```

答案：在函数声明中 `[static 1]` 表示要求实参至少是能够存储 $1$ 个元素的数组


**题目 5**：解释 `exit`、`quick_exit`、`_Exit`、`abort` 的区别。

答案: 这几个函数都是用于终止程序的。但是最好只是用 `exit` 函数。它们之间的区别如下

+ `exit` 函数用于终止程序。在程序正式结束前会调用 `atexit` 注册的清理函数，释放程序资源
+ `quick_exit` 函数用于快速终止程序。在程序终止前会调用 `at_quick_exit` 注册的函数
+ `_Exit` 函数用于终止程序，但是它只做平台清理
+ `abort` 函数立即终止程序，不做任何清理


**题目 6**：为什么 `fgetc` 返回 `int` 而不是 `char`？

答案: 因为 `fgetc` 会返回 `EOF`，它是一个负值。但是，`char` 是否带符号是由实现定义的，因此 `char` 可能无法表示 `EOF`

**题目 7**：以下代码的输出是什么？请逐步解释。

```c
unsigned char a = 255;
unsigned char b = 1;
unsigned char c = a + b;
printf("c = %u\n", (unsigned)c);
```

答案: 输出结果为 `c = 0`

+ 表达式 `a + b` 计算时会进行整数提示，此时 `a + b` 的结果为 256(int 类型)
+ 将 256 赋值给 `unsigned char` 类型的 `c` 时发生溢出，结果为 $256 \mod {2^{8 \times sizeof(unsigned char)}}$
+ 在我的平台上，`sizeof(unsigned char)` 的值为 $1$。因此，`c` 的值是 $0$

**题目 8**：解释 `sizeof` 和 `strlen` 的区别。

答案: 两者有两个关键区别

+ `sizeof` 是**运算符**，在**编译时**求值；`strlen` 是**函数**，在**运行时**求值
+ `sizeof` 计算对象或类型占用的**字节数**，**包含** `\0` 终止符；`strlen` 计算字符串的**字符数**，**不包含** `\0`

```c
char s[] = "hello";
sizeof(s);  // 6（5个字符 + '\0'）
strlen(s);  // 5
```

**题目 9**：以下代码有什么问题？

```c
char src[] = "hello";
char dst[3];
strcpy(dst, src);
```

答案: 缓冲区溢出。`strcpy` 会复制包括 `\0` 终止符在内的所有字符，因此需要 `strlen(src) + 1` 字节的空间。`src` 需要 6 字节（5个字符 + `\0`），而 `dst` 只有 3 字节，`strcpy` 会越界写入，这是未定义行为


**题目 10**：以下代码的输出是什么？为什么？

```c
int a = 5;
int b = 0;
int c = (b != 0) && (a / b > 1);
printf("c = %d\n", c);
```

答案: 输出内容是 `c = 0`。由于 `b` 的值是 $0$，`b != 0` 这个表达式不成立。运算符 `&&` 由短路特性，左表达式为 `false` 直接决定了整个表达式的值。输出结果为 `0`(`false`)


**题目 11**：`enum` 的成员值默认是什么？以下枚举中各成员的值是多少？

```c
enum Color { RED, GREEN = 10, BLUE, YELLOW = 20, CYAN };
```

答案: `enum` 成员的默认值是从 $0$ 开始。后一个成员默认情形下是前一个成员值加 $1$。`enum Color` 的每个枚举值如下

+ `RED == 0`
+ `GREEN == 10`
+ `BLUE == 11`
+ `YELLOW == 20`
+ `CYAN == 21`


**题目 12**：以下代码的输出是什么？请解释隐式类型转换的过程。

```c
int a = -1;
unsigned int b = 1;
if (a < b) {
    printf("a < b\n");
} else {
    printf("a >= b\n");
}
```

答案: 输出结果: `a >= b`。在表达式 `a < b` 中，混用了 `signed` 和 `unsigned`，此时编译器会进行隐式类型转换，将 `signed` 类型的值转换为 `unsigned` 类型。`-1` 的二进制补码表示（全 1）被重新解释为无符号整数，其值为 `UINT_MAX`（4294967295），大于 1，因此 `a < b` 为假

---

## 二、编程题

### 练习 1：字符串转整数（20 分钟）

实现类似 `atoi` 的函数，将字符串转换为整数。

**要求**：

- 处理前导空白字符
- 处理可选的正负号
- 处理数字字符
- 遇到非数字字符停止
- 处理溢出情况

**示例输出**：

```
输入: "  -123abc"
输出: -123

输入: "2147483648"
输出: 溢出
```

答案:

```c title="atoi.c"
/* atoi.c - 实现 atoi 将字符串转换为 int 类型的值 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum { BUFF_SIZE = 100 };

bool atoi(char const str[static 1], int* result);

int main(void) {
    char str[BUFF_SIZE];
    printf("输入: ");

    if (!fgets(str, BUFF_SIZE, stdin)) {
        perror("fgets:");
        return 0;
    }

    int result = {0};
    if (atoi(str, &result)) {
        printf("输出: %d\n", result);
    } else {
        fprintf(stderr, "输出: 溢出\n");
    }

    return 0;
}


bool atoi(char const str[static 1],  int* result) {
    *result = 0;
    if (!strlen(str)) {
        return true;
    }
    size_t start = strspn(str, " \n\f\r\v\t");

    // 处理正负号: 默认是正数
    int sign = 1;
    if (str[start] == '-') {
        sign *= -1;
        ++start;
    } else if (str[start] == '+') {
        ++start;
    }

    int base = 1;
    size_t end = start;
    while (str[end] && isdigit(str[end])) {
        ++end;
    };

    if (end == start) {
        *result = 0;
        return true;
    }

    // i >= start 可能会始终成立
    for (size_t i = end - 1; ; --i) {
        if (sign > 0) {
            *result += (str[i] - '0') * base;
        } else {
            *result -= (str[i] - '0') * base;
        }

        base *= 10;
        // 判断溢出
        if ((sign > 0 && *result < 0) || (sign < 0 && *result > 0)) {
            return false;
        }
        // 判断是否继续
        if (i == start) {
            break;
        }
    }
    return true;
}
```

---

### 练习 2：学生成绩管理（25 分钟）

编写程序，管理学生成绩。

**要求**：

- 使用结构体存储学生信息（姓名、学号、成绩）
- 输入 5 个学生的信息
- 计算平均分
- 找出最高分和最低分
- 按成绩排序并输出

**示例输出**：

```
请输入5个学生的信息：
学生1姓名: 张三
学生1学号: 1001
学生1成绩: 85
...
平均分: 82.4
最高分: 95 (李四)
最低分: 68 (王五)
排序后:
1. 李四 95
2. 张三 85
...
```

答案:

```c title="students.c"
/* student.c - 2：学生成绩管理 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum {NAME_SIZE = 20, NO_SIZE = 20, STUDENT_SIZE = 5};

typedef struct student Student;

struct student {
    char name[NAME_SIZE];
    char no[NO_SIZE];
    double grade;
};

double st_grade_avg(Student students[static 1], int size);
Student* st_grade_max(Student students[static 1], int size);
Student* st_grade_min(Student students[static 1], int size);
void st_sort(Student students[static 1], int size);

int main(void) {
    Student students[STUDENT_SIZE] = {};

    printf("请输入 %d 个学生信息:\n", STUDENT_SIZE);
    for (size_t i = 0; i < STUDENT_SIZE; ++i) {
        printf("学生 %zu 姓名: ", i + 1);
        fgets(students[i].name, NAME_SIZE, stdin);
        printf("学生 %zu 学号: ", i + 1);
        fgets(students[i].no, NO_SIZE, stdin);
        printf("学生 %zu 成绩: ", i + 1);
        fscanf(stdin, "%lf%*c", &students[i].grade); // %*c 消耗换行符
    }

    double avg = st_grade_avg(students, STUDENT_SIZE);
    printf("平均分: %.2f\n", avg);
    Student* max = st_grade_max(students, STUDENT_SIZE);
    Student* min = st_grade_min(students, STUDENT_SIZE);
    printf("最高分: %.2f\n", max->grade);
    printf("最低分: %.2f\n", min->grade);

    st_sort(students, STUDENT_SIZE);
    printf("排序后:\n");
    for (size_t i = 0; i < STUDENT_SIZE; ++i) {
        printf("%zu. %s %.2f", i+1, students[i].name, students[i].grade);
    }
    puts("");

    return 0;
}

double st_grade_avg(Student students[static 1], int size) {
    double result = {0};
    for (size_t i = 0; i < size; ++i) {
        result += students[i].grade;
    }
    return result / size;
}

Student* st_grade_max(Student students[static 1], int size) {
    Student* max = &students[0];
    for (size_t i = 1; i < size; ++i) {
        if (max->grade < students[i].grade) {
            max = &students[i];
        }
    }
    return max;
}
Student* st_grade_min(Student students[static 1], int size) {
    Student* min = &students[0];
    for (size_t i = 1; i < size; ++i) {
        if (min->grade > students[i].grade) {
            min = &students[i];
        }
    }
    return min;
}

int compar(const void* s1, const void* s2) {
    double diff = ((Student*)s1)->grade - ((Student*)s2)->grade;
    return (diff > 0) - (diff < 0);
}

void st_sort(Student students[static 1], int size) {
    qsort(students, size, sizeof(students[0]), compar);
}
```

---

### 练习 3：文件复制程序（15 分钟）

编写程序，将一个文件的内容复制到另一个文件。

**要求**：

- 使用命令行参数指定源文件和目标文件
- 使用 `fopen`、`fgets`、`fputs`、`fclose`
- 处理文件打开失败的情况
- 使用 `perror` 输出错误信息

**示例输出**：

```shell
$ ./copy source.txt dest.txt
复制完成！

$ ./copy nonexistent.txt dest.txt
无法打开源文件: No such file or directory
```

答案:

```c title="copy.c"
/* copy.c - 实现 cp 指令 */
#include <stdio.h>

int main(int argc, char* argv[argc + 1]) {

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <src> <dst>\n", argv[0]);
        return 1;
    }

    FILE * src = fopen(argv[1], "rb");
    if (!src) {
        perror("fopen");
        return 1;
    }
    FILE *dst = fopen(argv[2], "wb");
    if (!dst) {
        perror("fopen");
        fclose(src);
        return 1;
    }
    int ch = {0};
    while ((ch = fgetc(src)) != EOF) {
        if (fputc(ch, dst) == EOF) {
            perror("fputc:");
            break;
        }
    }
    if (feof(src)) {
        fclose(src);
        fclose(dst);
        return 0;
    } else {
        perror("fgetc");
        fclose(src);
        fclose(dst);
        return 1;
    }
}
```

---

### 练习 4：日期计算器（20 分钟）

编写程序，计算两个日期之间相差的天数。

**要求**：

- 输入两个日期（年、月、日）
- 使用 `struct tm` 和 `mktime`
- 处理闰年
- 输出两个日期相差的天数

**示例输出**：

```
请输入第一个日期（年 月 日）：2024 1 1
请输入第二个日期（年 月 日）：2024 12 31
两个日期相差 365 天
```

答案

```c title="datediff.c"
/* datediff.c - 计算两个日期相差的天数 */
#include <stdio.h>
#include <time.h>
#include <tgmath.h>

#define ONE_DAY_SEC (24 * 3600)

int main(void) {

    int year = 0;
    int month = 0;
    int day = 0;

    printf("请输入第一个日期（年 月 日）：");
    scanf("%d%d%d", &year, &month, &day);
    struct tm local1 = {
        .tm_mon=month-1,
        .tm_mday=day,
        .tm_year=year-1900
    };

    printf("请输入第二个日期（年 月 日）：");
    scanf("%d%d%d", &year, &month, &day);
    struct tm local2 = {
        .tm_mon=month-1,
        .tm_mday=day,  // 1 ~ 31
        .tm_year=year-1900
    };

    time_t t1 = mktime(&local1);
    time_t t2 = mktime(&local2);

    double diff = fabs(difftime(t1, t2));  // 两个日期相差多少秒
    double days = diff / ONE_DAY_SEC;

    printf("两个日期相差 %.0f天\n", days);

    return 0;
}
```

---

### 练习 5：冒泡排序（15 分钟）

编写程序，实现冒泡排序算法。

**要求**：

- 输入一个整数数组
- 使用冒泡排序算法进行排序
- 使用函数封装排序逻辑
- 输出排序前后的数组

**示例输出**：

```
请输入数组大小: 5
请输入5个整数: 64 25 12 22 11
排序前: 64 25 12 22 11
排序后: 11 12 22 25 64
```

答案:

```c title="bubble-sort.c"
/* bubble-sort.c - 冒泡排序 */
#include <stdio.h>

void bubble_sort(int array[static 1], int size);

int main(void) {
    int size = {0};
    printf("输入数组大小: ");
    scanf("%d%*c", &size);

    int array[size];
    printf("输入 %d 个整数: ", size);
    for (int i = 0; i < size; ++i) {
        scanf("%d", &array[i]);
    }

    printf("排序前:");
    for (int i = 0; i < size; ++i) {
        printf(" %d", array[i]);
    }
    puts("");
    bubble_sort(array, size);
    printf("排序后:");
    for (int i = 0; i < size; ++i) {
        printf(" %d", array[i]);
    }
    puts("");

    return 0;
}

void bubble_sort(int array[static 1], int size) {
    for (int i = 0; i < size; ++i) {
        bool exchanged = false;
        for (int j = 0; j < size - i - 1; ++j) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
                exchanged = true;
            }
        }
        if (!exchanged) {
            break;
        }
    }
}
```

---

### 练习 6：二分查找（15 分钟）

编写程序，实现二分查找算法。

**要求**：

- 输入一个已排序的整数数组
- 实现二分查找函数
- 返回目标值的索引，未找到返回 -1
- 使用递归和迭代两种方式实现

**示例输出**：

```
请输入数组大小: 7
请输入7个已排序的整数: 2 5 8 12 16 23 38
请输入要查找的值: 16
迭代法: 找到，索引 = 4
递归法: 找到，索引 = 4
```

答案:

```c title="binary-search.c"
/* binary-search.c - 二分查找 */
#include <stdio.h>

int binary_search_iter(int array[static 1], int size, int target);
int binary_search_recursion(int array[static 1], int size, int target);

int main(void) {

    int size = {0};
    int target = {0};

    printf("输入数组大小: ");
    scanf("%d", &size);
    int array[size];

    printf("输入 %d 个已排序的整数: ", size);
    for (int i = 0; i < size; ++i) {
        scanf("%d", &array[i]);
    }

    printf("输入目标值: ");
    scanf("%d", &target);

    int index = binary_search_iter(array, size, target);
    if (index != -1) {
        printf("迭代法: 找到, 索引 = %d\n", index);
    }

    index = binary_search_recursion(array, size, target);
    if (index != -1) {
        printf("递归法: 找到, 索引 = %d\n", index);
    }

    if (index == -1) {
        printf("未找到\n");
    }

    return 0;
}

int binary_search_iter(int array[static 1], int size, int target) {
    int left = 0;
    int right = size - 1;
    while (left <= right)
    {
        int mid = (left + right) / 2;
        if (array[mid] == target) {
            return mid;
        } else if (array[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

int binary_search_recursion2(int array[static 1], int left, int right, int target) {
    if (left > right) {
        return -1;
    }
    int mid = (left + right) / 2;
    if (array[mid] == target) {
        return mid;
    } else if (array[mid] < target) {
        return binary_search_recursion2(array, mid+1, right, target);
    } else {
        return binary_search_recursion2(array, left, mid - 1, target);
    }
}

int binary_search_recursion(int array[static 1], int size, int target) {
    return binary_search_recursion2(array, 0, size-1, target);
}
```

---

### 练习 7：递归与斐波那契数列（15 分钟）

编写程序，实现斐波那契数列。

**要求**：

- 使用递归和迭代两种方式实现
- 使用数组缓存优化递归（记忆化）
- 比较两种方法的效率

**示例输出**：

```
请输入 n: 10
斐波那契数列:
0 1 1 2 3 5 8 13 21 34

递归法: F(10) = 55
迭代法: F(10) = 55
记忆化: F(10) = 55
```

答案: 已在 [示例程序:斐波拉契数列](functions.md#示例程序斐波拉契数列) 和 [示例程序:斐波拉契数列的优化](functions.md#示例程序斐波拉契数列的优化) 和 [fibonacci 数列](phase1-exercise.md#练习-15fibonacci-数列10-分钟) 中实现。最好选用的是迭代法；它只需要记忆第 $i$ 项的前两个值，空间复杂度为 $O(1)$，时间复杂度为 $O(n)$

---

### 练习 8：位运算操作（20 分钟）

编写程序，实现以下位操作功能。

**要求**：

- 判断一个数是否为 2 的幂
- 统计一个无符号整数的二进制表示中 1 的个数
- 交换两个变量的值（不使用临时变量）
- 将一个整数的第 n 位设为 1，第 m 位清零

**示例输出**：

```
16 是 2 的幂: 是
7 中 1 的个数: 3
交换前: a=5, b=9
交换后: a=9, b=5
将 0b1010 的第 1 位设为 1: 0b1010
将 0b1111 的第 2 位清零: 0b1011
```

答案：

```c title="bitop.c"
/* bitop.c - 位操作 */
#include <stdio.h>


bool is_power_of_two(int number) {
    if (number == 0) {
        return false;
    }

    if (number < 0) {
        number = -number;
    }
    return !(number & (number - 1));
}

size_t one_bit_count(unsigned int number) {
    size_t count = 0;
    while (number > 0)
    {
        (void)(number & 0x1 ? ++count : 0);
        number >>= 1;
    }
    return count;
}

void swap(int *a, int *b) {
    *a ^= *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

unsigned set_bit(unsigned a, int n) {
    return a | ((unsigned)1 << n);
}

unsigned clear_bit(unsigned a, int n) {
    return a & ~((unsigned)1 << n);
}

void print_binary(unsigned int number) {
    for (int i = 8 * sizeof(unsigned int) - 1; i >= 0; --i) {
        printf("%d", (number >> i) & 1);
    }
}

int main(void) {
    /* is_power_of_two */
    printf("16 是 2 的幂: %s\n", is_power_of_two(16) ? "是" : "否");
    printf("7 是 2 的幂: %s\n", is_power_of_two(7) ? "是" : "否");

    /* one_bit_count */
    printf("7 中 1 的个数: %zu\n", one_bit_count(7));
    printf("255 中 1 的个数: %zu\n", one_bit_count(255));

    /* swap */
    int a = 5, b = 9;
    printf("交换前: a=%d, b=%d\n", a, b);
    swap(&a, &b);
    printf("交换后: a=%d, b=%d\n", a, b);

    /* set_bit */
    unsigned val = 0b1010;
    printf("将 0b");
    print_binary(val);
    printf(" 的第 1 位设为 1: 0b");
    print_binary(set_bit(val, 1));
    printf("\n");

    /* clear_bit */
    val = 0b1111;
    printf("将 0b");
    print_binary(val);
    printf(" 的第 2 位清零: 0b");
    print_binary(clear_bit(val, 2));
    printf("\n");

    return 0;
}
```

---

### 练习 9：字符串操作函数（20 分钟）

不使用 `<string.h>`，手动实现以下字符串操作函数。

**要求**：

- `my_strlen` — 计算字符串长度
- `my_strcpy` — 复制字符串
- `my_strcat` — 拼接字符串
- `my_strcmp` — 比较两个字符串
- `my_strchr` — 查找字符首次出现的位置

**示例输出**：

```
my_strlen("hello") = 5
my_strcpy: "hello world"
my_strcat: "hello world"
my_strcmp("abc", "abd") = -1
my_strchr("hello", 'l') = "llo"
```

答案:

```c title="strings.c"
/* strings.c - 实现 str 相关的函数
- `my_strlen` — 计算字符串长度
- `my_strcpy` — 复制字符串
- `my_strcat` — 拼接字符串
- `my_strcmp` — 比较两个字符串
- `my_strchr` — 查找字符首次出现的位置
*/

#include <stddef.h>
#include <stdio.h>

size_t my_strlen(char const str[static 1]) {
    size_t len = 0;
    for (; str[len]; ++len);
    return len;
}

char* my_strcpy(char dst[static 1], char const src[static 1]) {

    size_t i = 0;
    for (i = 0; src[i]; ++i) {
        dst[i] = src[i];
    }
    dst[i] = '\0';
    return dst;
}

char* my_strcat(char dst[static 1], char const src[static 1]) {
    size_t start = my_strlen(dst);
    size_t end = start;
    for (size_t i = 0; src[i]; ++end,++i) {
        dst[end] = src[i];
    }
    dst[end] = '\0';
    return dst;
}

int my_strcmp(char const str1[static 1], char const  str2[static 1]) {
    size_t i = 0;
    int result = 0;
    for (i = 0; str1[i] && str2[i]; ++i) {
        result = (unsigned char)str1[i] - (unsigned char)str2[i];  // 类型提升为 int 不会有溢出风险
        if (result != 0) {
            return result;
        }
    }

    return (unsigned char)str1[i] - (unsigned char)str2[i];
}

char const* my_strchr(char const str[static 1], int ch) {
    int result = 0;
    for (result = 0; str[result]; ++result) {
        if (str[result] == ch) {
            return &str[result];
        }
    }
    return nullptr;
}

int main(void) {
    /* my_strlen */
    printf("my_strlen(\"hello\") = %zu\n", my_strlen("hello"));

    /* my_strcpy */
    char dst[64] = {};
    my_strcpy(dst, "hello world");
    printf("my_strcpy: \"%s\"\n", dst);

    /* my_strcat */
    my_strcpy(dst, "hello");
    my_strcat(dst, " world");
    printf("my_strcat: \"%s\"\n", dst);

    /* my_strcmp */
    printf("my_strcmp(\"abc\", \"abd\") = %d\n", my_strcmp("abc", "abd"));
    printf("my_strcmp(\"abc\", \"abc\") = %d\n", my_strcmp("abc", "abc"));
    printf("my_strcmp(\"abd\", \"abc\") = %d\n", my_strcmp("abd", "abc"));

    /* my_strchr */
    char const *p = my_strchr("hello", 'l');
    printf("my_strchr(\"hello\", 'l') = \"%s\"\n", p ? p : "(null)");
    p = my_strchr("hello", 'z');
    printf("my_strchr(\"hello\", 'z') = %s\n", p ? p : "(null)");

    return 0;
}
```

---

### 练习 10：枚举与复合字面量（15 分钟）

编写程序，使用枚举和复合字面量实现一个简易的形状面积计算器。

**要求**：

- 使用枚举表示形状类型（圆形、矩形、三角形）
- 使用结构体存储形状数据
- 使用复合字面量创建形状
- 使用 `switch` 计算不同形状的面积

**示例输出**：

```
圆形（半径=5.0）面积: 78.54
矩形（宽=4.0, 高=6.0）面积: 24.00
三角形（底=3.0, 高=8.0）面积: 12.00
```

答案:

```c title="enum-composite.c"
/* enum-composite.c - 枚举与复合字面量 */
#define _GNU_SOURCE
#include <stdio.h>
#include <math.h>

enum Shape {CIRCULAR, RECTANGLE, TRIANGLE} ;

typedef struct {
    enum Shape type;
    union {
        struct  {
            double radius;
        } circular;
        struct {
            double width;
            double height;
        } rectangle;
         struct  {
            double base;
            double height;
        } triangle;
    } shape;
} Shape;


double area(Shape shape) {
    switch (shape.type)
    {
    case CIRCULAR:
        return M_PI * shape.shape.circular.radius * shape.shape.circular.radius;
    case RECTANGLE:
        return shape.shape.rectangle.width * shape.shape.rectangle.height;

    case TRIANGLE:

        return (shape.shape.triangle.base * shape.shape.triangle.height) / 2;
    }
    return 0;
}

int main(void) {
    printf("圆形（半径=5.0）面积: %.2f\n", area((Shape){.type=CIRCULAR, .shape={.circular.radius=5.0}}));
    printf("矩形（宽=4.0，高=6.0）面积: %.2f\n", area((Shape){.type=RECTANGLE, .shape={.rectangle.height=6.0, .rectangle.width=4.0}}));
    printf("三角形（底=3.0，高=8.0）面积: %.2f\n", area((Shape){.type=TRIANGLE, .shape={.triangle.base=3.0, .triangle.height=8.0}}));

    return 0;
}
```

---

### 练习 11：整数进制转换器（20 分钟）

编写程序，将整数在不同进制之间转换。

**要求**：

- 将十进制整数转换为二进制、八进制、十六进制字符串
- 将二进制、八进制、十六进制字符串转换为十进制整数
- 使用手动算法实现，不直接使用 `printf` 的 `%x` `%o` 格式
- 处理负数

**示例输出**：

```
255 -> 二进制: 11111111
255 -> 八进制: 377
255 -> 十六进制: ff
11111111 -> 十进制: 255
377 -> 十进制: 255
ff -> 十进制: 255
```

答案:

```c title="base-convert.c"
/* base-convert.c - 整数进制转换器 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* 十进制转任意进制字符串（2/8/16） */
void dec_to_base(unsigned int number, int base, char buf[static 65]) {
    if (number == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    char const digits[] = "0123456789abcdef";
    char tmp[65];
    int len = 0;

    while (number > 0) {
        tmp[len++] = digits[number % base];
        number /= base;
    }

    /* 反转 */
    for (int i = 0; i < len; ++i) {
        buf[i] = tmp[len - 1 - i];
    }
    buf[len] = '\0';
}

/* 任意进制字符串转十进制（2/8/16） */
unsigned int base_to_dec(char const str[static 1], int base) {
    unsigned int result = 0;
    for (size_t i = 0; str[i]; ++i) {
        int digit = 0;
        if (isdigit((unsigned char)str[i])) {
            digit = str[i] - '0';
        } else {
            digit = tolower((unsigned char)str[i]) - 'a' + 10;
        }
        result = result * base + digit;
    }
    return result;
}

int main(void) {
    char buf[65] = {};
    int number = 0;

    /* 十进制 -> 其他进制 */
    printf("输入一个十进制整数: ");
    scanf("%d", &number);

    unsigned int abs_val = (number < 0) ? -(unsigned int)number : (unsigned int)number;
    char const *sign = (number < 0) ? "-" : "";

    dec_to_base(abs_val, 2, buf);
    printf("%d -> 二进制: %s%s\n", number, sign, buf);

    dec_to_base(abs_val, 8, buf);
    printf("%d -> 八进制: %s%s\n", number, sign, buf);

    dec_to_base(abs_val, 16, buf);
    printf("%d -> 十六进制: %s%s\n", number, sign, buf);

    /* 其他进制 -> 十进制 */
    printf("\n二进制 11111111 -> 十进制: %u\n", base_to_dec("11111111", 2));
    printf("八进制 377 -> 十进制: %u\n", base_to_dec("377", 8));
    printf("十六进制 ff -> 十进制: %u\n", base_to_dec("ff", 16));

    return 0;
}
```

<details>
<summary><strong>NOTE: 编译并运行</strong></summary>

```shell
➜ gcc -Wall -Wextra -std=c23 -o base-convert base-convert.c
➜ ./base-convert
输入一个十进制整数: 255
255 -> 二进制: 11111111
255 -> 八进制: 377
255 -> 十六进制: ff

二进制 11111111 -> 十进制: 255
八进制 377 -> 十进制: 255
十六进制 ff -> 十进制: 255
➜ ./base-convert
输入一个十进制整数: -42
-42 -> 二进制: -101010
-42 -> 八进制: -52
-42 -> 十六进制: -2a

二进制 11111111 -> 十进制: 255
八进制 377 -> 十进制: 255
十六进制 ff -> 十进制: 255
```

</details>

---

## 三、思考题

**思考题 1**：为什么 C 语言中数组作为函数参数会退化为指针？这样设计有什么优缺点？

答案：C 语言参考了 B 语言的设计。将数组退化为指针传递时不用拷贝所有数组成员。提升传递数组参数的效率。但是，函数可以直接修改实参的值。

**思考题 2**：`assert` 和 `static_assert` 的区别是什么？分别在什么场景下使用？

答案: `assert` 是运行时断言，而 `static_assert` 是静态断言

+ `assert`：条件要在运行时才能确定的情况下使用
+ `static_assert`：条件在编译时确定的情况下进行编译时断言


**思考题 3**：为什么 C 语言没有垃圾回收机制？这对程序员有什么影响？

答案：C 语言设计之初时用于开发操作系统的。这需要语言保持高性能，引入垃圾回收机制势必会影响程序性能，不符合设计要求。

**思考题 4**：以下代码在不同平台上可能输出不同的结果，为什么？

```c
printf("%zu\n", sizeof(int));
printf("%zu\n", sizeof(long));
```

答案：C 标准没有规定基本类型的尺寸，只规定了类型范围的包含关系。不同的平台上，`int` 和 `long` 的尺寸可能相同（例如，Windows），但是它们也是两种类型


**思考题 5**：为什么以下代码的结果可能不符合预期？

```c
double d = 0.1 + 0.2;
if (d == 0.3) {
    printf("equal\n");
} else {
    printf("not equal: %.20f\n", d);
}
```

答案: 十进制小数使用二进制表示时，由于二进制位模式长度限制，导致无法完整的表示。因此，计算机上的浮点数是无法精确表示一个十进制小数的。

**思考题 6**：`#define SQUARE(x) x * x` 这个宏有什么问题？请给出正确写法。

答案:
1. `x` 只是简单文本，不是值。应该将 `x` 包含在括号内。
2. 出现 `SQUARE(x)` 的位置只是简单替换成替换文本，可能由运算符优先级导致运算符结合问题。应该将替换文本包含在括号内

最终的结果为

```c
#define SQUARE(x) ((x) * (x))
```

---

## 四、挑战题

### 挑战 1：实现简易版 `printf`（30 分钟）

实现一个简化版的 `printf`，支持以下格式说明符：

- `%d` — 十进制整数
- `%x` — 十六进制整数
- `%s` — 字符串
- `%c` — 字符
- `%%` — 字面 `%`

**要求**：

- 使用可变参数函数（`<stdarg.h>`）
- 处理格式字符串中的普通字符
- 不需要处理宽度、精度等修饰符

**示例**：

```c
my_printf("Hello, %s! You are %d years old.\n", "Alice", 25);
// 输出: Hello, Alice! You are 25 years old.
```

### 挑战 2：实现链表（30 分钟）

实现一个单链表，支持以下操作：

- 创建节点
- 头部插入
- 尾部插入
- 删除节点
- 查找节点
- 打印链表
- 反转链表

**示例**：

```c
List list = create_list();
push_front(&list, 3);
push_front(&list, 2);
push_front(&list, 1);
push_back(&list, 4);
print_list(&list);  // 1 -> 2 -> 3 -> 4 -> NULL
reverse(&list);
print_list(&list);  // 4 -> 3 -> 2 -> 1 -> NULL
```
