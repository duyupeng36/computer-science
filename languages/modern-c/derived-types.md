# 派生类型

在 C 语言中的所有其他数据类型都是从我们现在知道的 [基本类型] 派生出来的；派生数据类型有 **四种策略**

| 派生方式                | 说明                                       |
| ----------------------- | ------------------------------------------ |
| **数组（Array）**       | 同一类型的多个对象聚合                     |
| **指针（Pointer）**     | 引用内存中的对象（本章只做不透明类型介绍） |
| **结构体（Structure）** | 不同类型的多个对象聚合                     |
| **联合体（Union）**     | 不同类型叠加在同一内存位置                 |

下面我们会重点学习 **数组** 和 **结构体**。指针是目前为止最复杂的概念，我们将花费两周的时间来学习 [指针]。这里，我们将只把它作为不透明数据类型来讨论，甚至没有提到它们达到的真正目的。联合体涉及了 C 语言中的内存模型，放在 [C 内存模型] 中介绍

## 数组 {#array}

数组是 C 语言中最基本的 **聚合数据类型**，它将多个 **相同类型** 的对象组合成一个更大的对象。在 C 语言中，数组和指针密切相关；数组在很多上下文中看起来很像指针。为了解释清楚，我们先从数组开始，并在正式学习指针之前尽可能的使用数组

> [!TIP]
> 数组是一个由 $N$ 个相同类型元素组成的 **连续内存块**

### 数组声明 {#array-declaration}

在 C 语言中，为了声明数组，需要指明数组元素的 **类型** 和 **数量**。例如，为了声明数组 `a` 有 $10$ 个 `int` 类型的元素，可以写成

```c
int a[10];
```

数组的大小 $10$ 称为 **数组长度**，它必须是一个 **整数常量表达式（ICE）**，即在编译时就能确定的值

```c
#define N 10
constexpr int M = 20;

int a[N];    // ✅ 宏常量
int b[M];    // ✅ constexpr 常量
int c[10];   // ✅ 字面量

int n = 10;
int d[n];    // ⚠️ C99 VLA（变长数组），C23 已弃用
```

> [!WARNING]
> C23 标准已弃用变长数组（VLA）。虽然部分编译器仍支持，但不建议在新代码中使用

### 数组初始化 {#array-initialization}

数组的初始化器必须用 `{...}` 包围，元素之间用逗号分隔。C 语言提供了多种初始化方式

#### 顺序初始化

```c
int a[5] = {1, 2, 3, 4, 5};       // 完全初始化
int b[5] = {1, 2, 3};              // 部分初始化，b = {1, 2, 3, 0, 0}
int c[5] = {0};                    // 所有元素为 0
int d[] = {1, 2, 3};               // 省略大小，编译器推导为 3
double e[] = {1.0, 2.5, 3.7};     // double 数组，大小为 3
```

#### 指示器初始化（C99）

指示器 `[index]` 明确指定每个值对应的下标，未被覆盖的元素自动初始化为 $0$

```c
int a[5] = {[0] = 1, [3] = 4, [4] = 5};
// a = {1, 0, 0, 4, 5}

int b[5] = {[3] = 30, [0] = 10, [2] = 20};
// 乱序初始化：b = {10, 0, 20, 30, 0}

int c[5] = {[0] = 1, [0] = 2};
// 重复指定：后面的覆盖前面的，c = {2, 0, 0, 0, 0}
```

> [!TIP]
> 指示器初始化的优势：不依赖位置，更健壮。如果后来在前面插入一个元素，其他元素不受影响

#### C23 默认初始化

```c
int a[10] = {};     // 所有元素为 0
int b[10] = {0};    // 等价写法（C89 起）
```

### 数组访问 {#array-access}

使用 **下标运算符 `[]`** 访问数组元素，下标从 $0$ 开始

```c
int a[5] = {10, 20, 30, 40, 50};

a[0]     // 10（第一个元素）
a[4]     // 50（最后一个元素）
a[2]     // 30
a[5]     // ❌ 越界！未定义行为
a[-1]    // ❌ 越界！未定义行为
```

> [!CAUTION]
> C 语言 **不检查数组越界**。访问 `a[5]` 或 `a[-1]` 是未定义行为，可能导致程序崩溃或产生错误结果

#### 数组遍历

```c
int a[5] = {10, 20, 30, 40, 50};

// 方式 1：下标遍历
for (int i = 0; i < 5; ++i) {
    printf("%d ", a[i]);
}

// 方式 2：指针遍历（更高效）
for (int *p = a; p < a + 5; ++p) {
    printf("%d ", *p);
}

// 方式 3：C99 指定下标范围遍历
for (int i = 0; i < 5; ++i) {
    printf("%d ", a[i]);
}
```

### 数组与内存 {#array-and-memory}

数组元素在内存中 **连续存储**，相邻元素的地址差为元素大小

```c
int a[5];

printf("a 的地址:   %p\n", (void *)a);
printf("a[0] 的地址: %p\n", (void *)&a[0]);
printf("a[1] 的地址: %p\n", (void *)&a[1]);
printf("a[2] 的地址: %p\n", (void *)&a[2]);
```

输出示例（64 位系统）：
```
a 的地址:   0x7fff5a3b6c40
a[0] 的地址: 0x7fff5a3b6c40
a[1] 的地址: 0x7fff5a3b6c44    // +4（int 占 4 字节）
a[2] 的地址: 0x7fff5a3b6c48    // +4
```

> [!TIP]
> 数组名 `a` 在大多数上下文中会 **退化为指向首元素的指针** `&a[0]`。但 `sizeof(a)` 返回的是整个数组的大小，不是指针的大小

```c
int a[10];

sizeof(a);      // 40（10 × 4 字节）
sizeof(a[0]);   // 4（单个元素大小）
sizeof(a) / sizeof(a[0]);  // 10（数组长度）
```

### 二维数组 {#2d-array}

二维数组可以看作 **数组的数组**

```c
int matrix[3][4];  // 3 行 4 列

// 初始化
int matrix[3][4] = {
    {1,  2,  3,  4},    // 第 0 行
    {5,  6,  7,  8},    // 第 1 行
    {9, 10, 11, 12}     // 第 2 行
};

// 使用指示器
int matrix[3][4] = {
    [0][0] = 1, [0][3] = 4,
    [1][1] = 6,
    [2][2] = 11
};
```

#### 二维数组的内存布局

二维数组在内存中也是 **行优先** 连续存储

```
matrix[0][0] → matrix[0][1] → matrix[0][2] → matrix[0][3]
    ↓
matrix[1][0] → matrix[1][1] → matrix[1][2] → matrix[1][3]
    ↓
matrix[2][0] → matrix[2][1] → matrix[2][2] → matrix[2][3]
```

#### 二维数组遍历

```c
int matrix[3][4] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12}
};

// 行优先遍历（推荐，缓存友好）
for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 4; ++j) {
        printf("%4d", matrix[i][j]);
    }
    printf("\n");
}
```

### 数组作为函数参数 {#array-as-parameter}

当数组作为函数参数时，它会 **退化为指针**，丢失长度信息

```c
// ❌ 错误：数组退化为指针，sizeof 不再是数组大小
void print_array(int a[]) {
    printf("size: %zu\n", sizeof(a));  // 8（指针大小），不是 40
}

// ✅ 正确：显式传递长度
void print_array(int a[], size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

int main(void) {
    int arr[5] = {1, 2, 3, 4, 5};
    print_array(arr, 5);  // 传数组名 + 长度
}
```

> [!TIP]
> 函数参数中的 `int a[]` 和 `int *a` 是完全等价的，都表示指向 `int` 的指针

### 示例程序：冒泡排序 {#bubble-sort}

```c
#include <stdio.h>

void bubble_sort(int a[], size_t len) {
    for (size_t i = 0; i < len - 1; ++i) {
        int swapped = 0;
        for (size_t j = 0; j < len - 1 - i; ++j) {
            if (a[j] > a[j + 1]) {
                // 交换
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) break;  // 没有交换说明已排序
    }
}

void print_array(int a[], size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

int main(void) {
    int a[] = {64, 34, 25, 12, 22, 11, 90};
    size_t len = sizeof(a) / sizeof(a[0]);

    printf("排序前: ");
    print_array(a, len);

    bubble_sort(a, len);

    printf("排序后: ");
    print_array(a, len);

    return 0;
}
```

---

## 结构体 {#structure}

结构体是 C 语言中最重要的 **聚合数据类型**，它允许我们将 **不同类型** 的数据组合成一个整体

> [!TIP]
> 结构体就是把多个不同类型的变量打包在一起，形成一个新的自定义类型

### 结构体声明 {#struct-declaration}

```c
// 方式 1：先声明类型，再定义变量
struct Point {
    double x;
    double y;
};

struct Point p1;  // 定义变量

// 方式 2：声明类型的同时定义变量
struct Point {
    double x;
    double y;
} p1, p2;  // 同时定义了 p1 和 p2

// 方式 3：使用 typedef（推荐）
typedef struct {
    double x;
    double y;
} Point;

Point p1;  // 不需要写 struct 关键字
```

### 结构体初始化 {#struct-initialization}

```c
typedef struct {
    int id;
    char name[50];
    double score;
} Student;

// 方式 1：按顺序初始化
Student s1 = {1, "Alice", 95.5};

// 方式 2：C99 指示器初始化（推荐）
Student s2 = {
    .id = 2,
    .name = "Bob",
    .score = 88.0
};

// 方式 3：C23 默认初始化
Student s3 = {};  // 所有成员为 0
```

> [!TIP]
> 指示器初始化的优势：不依赖成员顺序，代码可读性更好。如果后来在结构体中间插入一个成员，其他成员的初始化不受影响

### 结构体成员访问 {#struct-member-access}

使用 **`.` 运算符** 访问结构体成员

```c
Student s1 = {.id = 1, .name = "Alice", .score = 95.5};

printf("ID: %d\n", s1.id);        // 1
printf("Name: %s\n", s1.name);    // Alice
printf("Score: %.1f\n", s1.score); // 95.5

// 修改成员
s1.score = 98.0;
```

### 结构体数组 {#struct-array}

```c
Student class[3] = {
    {.id = 1, .name = "Alice", .score = 95.5},
    {.id = 2, .name = "Bob", .score = 88.0},
    {.id = 3, .name = "Charlie", .score = 92.0}
};

// 遍历结构体数组
for (int i = 0; i < 3; ++i) {
    printf("Student %d: %s, Score: %.1f\n",
           class[i].id, class[i].name, class[i].score);
}
```

### 嵌套结构体 {#nested-struct}

```c
typedef struct {
    int year;
    int month;
    int day;
} Date;

typedef struct {
    int id;
    char name[50];
    Date birthday;   // 嵌套结构体
} Student;

Student s = {
    .id = 1,
    .name = "Alice",
    .birthday = {.year = 2000, .month = 5, .day = 20}
};

printf("Birthday: %d-%02d-%02d\n",
       s.birthday.year, s.birthday.month, s.birthday.day);
```

### 结构体与函数 {#struct-and-function}

#### 结构体作为函数参数

```c
// 值传递（会复制整个结构体，效率低）
void print_student(Student s) {
    printf("ID: %d, Name: %s\n", s.id, s.name);
}

// 指针传递（推荐，只传递地址）
void print_student_ptr(const Student *s) {
    printf("ID: %d, Name: %s\n", s->id, s->name);
    // 注意：用 -> 运算符访问指针指向的结构体成员
}

int main(void) {
    Student s = {.id = 1, .name = "Alice", .score = 95.5};

    print_student(s);        // 值传递
    print_student_ptr(&s);   // 指针传递
}
```

> [!WARNING]
> 结构体作为函数参数时，**推荐使用指针传递**。值传递会复制整个结构体，对于大型结构体效率很低

#### 结构体作为函数返回值

```c
// 返回结构体（会复制）
Student create_student(int id, const char *name, double score) {
    Student s = {
        .id = id,
        .score = score
    };
    // 注意：字符串不能直接赋值，需要用 strcpy
    strcpy(s.name, name);
    return s;
}

// 返回指针（⚠️ 不要返回局部变量的指针！）
Student *bad_create(void) {
    Student s = {1, "Alice", 95.5};
    return &s;  // ❌ 未定义行为！s 是局部变量
}
```

> [!CAUTION]
> **不要返回局部变量的指针或引用！** 局部变量在函数返回后会被销毁，返回其指针会导致悬空指针

#### 结构体指针运算符 `->`

当有一个指向结构体的指针时，使用 `->` 运算符访问成员

```c
typedef struct {
    int x;
    int y;
} Point;

void move(Point *p, int dx, int dy) {
    p->x += dx;   // 等价于 (*p).x += dx
    p->y += dy;
}

int main(void) {
    Point p = {10, 20};
    move(&p, 5, 3);
    printf("(%d, %d)\n", p.x, p.y);  // (15, 23)
}
```

### 结构体大小与对齐 {#struct-size-alignment}

结构体的大小不仅仅是成员大小之和，还涉及 **内存对齐**

```c
typedef struct {
    char a;     // 1 字节
    int b;      // 4 字节
    char c;     // 1 字节
} Bad;

typedef struct {
    int b;      // 4 字节
    char a;     // 1 字节
    char c;     // 1 字节
    // 2 字节填充（padding）
} Good;

printf("Bad size:  %zu\n", sizeof(Bad));   // 12（有 6 字节填充）
printf("Good size: %zu\n", sizeof(Good));  // 8（只有 2 字节填充）
```

> [!TIP]
> 为了减少内存浪费，**按成员大小从大到小排列**。这是结构体对齐的最佳实践

#### `offsetof` 宏

```c
#include <stddef.h>

typedef struct {
    char a;
    int b;
    char c;
} S;

printf("offsetof(S.a) = %zu\n", offsetof(S, a));  // 0
printf("offsetof(S.b) = %zu\n", offsetof(S, b));  // 4
printf("offsetof(S.c) = %zu\n", offsetof(S, c));  // 8
printf("sizeof(S) = %zu\n", sizeof(S));            // 12
```

### 位域 {#bit-field}

位域允许我们指定结构体成员占用的 **位数**，用于节省内存

```c
typedef struct {
    unsigned int year : 12;   // 12 位，范围 0~4095
    unsigned int month : 4;   // 4 位，范围 0~15
    unsigned int day : 5;     // 5 位，范围 0~31
} CompactDate;

printf("sizeof(CompactDate) = %zu\n", sizeof(CompactDate));  // 4
```

> [!NOTE]
> 位域主要用于硬件寄存器操作和网络协议解析。日常编程中使用较少

---

## 联合体 {#union}

联合体与结构体类似，但所有成员 **共享同一块内存**

```c
typedef union {
    int i;
    float f;
    char str[20];
} Value;

printf("sizeof(Value) = %zu\n", sizeof(Value));  // 20（最大成员的大小）
```

#### 联合体的应用：类型标签

```c
typedef enum { INT, FLOAT, STRING } Type;

typedef struct {
    Type type;
    union {
        int i;
        float f;
        char str[20];
    } value;
} TaggedValue;

void print_value(const TaggedValue *tv) {
    switch (tv->type) {
        case INT:    printf("int: %d\n", tv->value.i); break;
        case FLOAT:  printf("float: %.2f\n", tv->value.f); break;
        case STRING: printf("string: %s\n", tv->value.str); break;
    }
}

TaggedValue v1 = {.type = INT, .value.i = 42};
TaggedValue v2 = {.type = FLOAT, .value.f = 3.14};
TaggedValue v3 = {.type = STRING};
strcpy(v3.value.str, "hello");

print_value(&v1);  // int: 42
print_value(&v2);  // float: 3.14
print_value(&v3);  // string: hello
```

> [!WARNING]
> 联合体同一时刻只能存储一个成员的值。写入一个成员后，其他成员的值是 **未定义的**

---

## 枚举 {#enum}

枚举用于定义 **命名的整数常量** 集合

```c
// 基本枚举
enum Color { RED, GREEN, BLUE };
// RED = 0, GREEN = 1, BLUE = 2

// 指定值
enum Color { RED = 1, GREEN = 5, BLUE = 10 };

// C23 指定底层类型
enum Color : unsigned char { RED, GREEN, BLUE };
```

#### 枚举的使用

```c
enum Color { RED, GREEN, BLUE };

void print_color(enum Color c) {
    switch (c) {
        case RED:   printf("Red\n"); break;
        case GREEN: printf("Green\n"); break;
        case BLUE:  printf("Blue\n"); break;
    }
}

enum Color c = RED;
print_color(c);  // Red
```

> [!TIP]
> 枚举比 `#define` 更好：枚举有类型检查，调试时能看到名字，作用域受限

---

## 自引用结构体 {#self-referential}

结构体可以包含指向 **自身类型** 的指针，这是实现链表、树等数据结构的基础

```c
// 链表节点
typedef struct Node {
    int data;
    struct Node *next;  // 指向同类型结构体的指针
} Node;

// 二叉树节点
typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;
```

> [!WARNING]
> 自引用结构体只能包含指向自身类型的 **指针**，不能包含自身类型的 **值**（会导致无限递归）

---

## 示例程序：学生成绩管理系统 {#student-management}

```c
#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100

typedef struct {
    int id;
    char name[50];
    double scores[3];  // 三门课成绩
} Student;

typedef struct {
    Student students[MAX_STUDENTS];
    int count;
} Classroom;

// 添加学生
void add_student(Classroom *cls, int id, const char *name,
                 double s1, double s2, double s3) {
    if (cls->count >= MAX_STUDENTS) {
        printf("班级已满！\n");
        return;
    }
    Student *s = &cls->students[cls->count++];
    s->id = id;
    strcpy(s->name, name);
    s->scores[0] = s1;
    s->scores[1] = s2;
    s->scores[2] = s3;
}

// 计算平均分
double average(const Student *s) {
    return (s->scores[0] + s->scores[1] + s->scores[2]) / 3.0;
}

// 打印所有学生
void print_classroom(const Classroom *cls) {
    printf("%-5s %-15s %8s %8s %8s %8s\n",
           "ID", "Name", "Math", "Eng", "CS", "Avg");
    printf("------------------------------------------------------\n");
    for (int i = 0; i < cls->count; ++i) {
        const Student *s = &cls->students[i];
        printf("%-5d %-15s %8.1f %8.1f %8.1f %8.1f\n",
               s->id, s->name,
               s->scores[0], s->scores[1], s->scores[2],
               average(s));
    }
}

// 按平均分排序（降序）
void sort_by_average(Classroom *cls) {
    for (int i = 0; i < cls->count - 1; ++i) {
        for (int j = 0; j < cls->count - 1 - i; ++j) {
            if (average(&cls->students[j]) <
                average(&cls->students[j + 1])) {
                Student temp = cls->students[j];
                cls->students[j] = cls->students[j + 1];
                cls->students[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    Classroom cls = {.count = 0};

    add_student(&cls, 1, "Alice", 90, 85, 92);
    add_student(&cls, 2, "Bob", 78, 88, 80);
    add_student(&cls, 3, "Charlie", 95, 92, 98);
    add_student(&cls, 4, "Diana", 88, 75, 85);

    printf("=== 成绩单 ===\n");
    print_classroom(&cls);

    sort_by_average(&cls);
    printf("\n=== 按平均分排序 ===\n");
    print_classroom(&cls);

    return 0;
}
```

---

## 总结 {#summary}

| 类型 | 用途 | 内存 | 访问方式 |
|------|------|------|---------|
| 数组 | 相同类型元素集合 | 连续 | `a[i]` |
| 结构体 | 不同类型元素聚合 | 连续（有填充） | `s.member` |
| 联合体 | 不同类型共享内存 | 最大成员大小 | `u.member` |
| 枚举 | 命名整数常量 | int 大小 | 直接使用 |
| 位域 | 按位指定成员大小 | 按需 | `s.field` |

---

## 练习题 {#exercises}

1. 实现一个函数，对整数数组进行冒泡排序
2. 定义一个 `Matrix` 结构体（包含行数、列数、数据），实现矩阵加法
3. 用结构体实现一个简单的学生信息管理系统（增删改查）
4. 用链表（自引用结构体）实现栈
5. 用枚举 + 联合体实现一个简单的 "类型安全的变体类型"

[基本类型]: basic-types.md#basic-types
[指针]: pointer.md
[C 内存模型]: memory-model.md
