# 阶段3-练习

> 涵盖：代码风格、组织与文档、指针、C 内存模型、存储、更复杂的处理和IO、程序异常

---

## 一、代码风格

### 基础概念题（口头回答）

**题目 1**【代码风格】：以下代码有哪些风格问题？

```c
const char *GetName(int ID){
if(ID<0)
return NULL;
char * name=(char*)malloc(100);
return name;
}
```

答案

+ 参数名 `ID` 应该小写 `id`，不要与宏定义混淆
+ `if` 与 `()` 之间应该保留一个空格
+ `if` 的依赖块应该放在 `{}` 内，并使用缩进
+ `=` 两个应该保留空格
+ `*` 表示指针时应该与类型放在一起

修改后

```c
char const* GetName(int id){
    if (id < 0) {
        return NULL;
    }
    char* name = (char*)malloc(100);
    return name;
}
```

**题目 2**【代码风格】：以下标识符哪些违反了 C 标准的保留规则？

```c
int _count;           // (a)
char *str_cpy;        // (b)
typedef int my_int_t; // (c)
#define MAX_SIZE 100   // (d)
int __internal;       // (e)
double E_value;       // (f)
```

答案

+ `(a)`: 没有违反规则
+ `(b)`：`str` 开头的标识符 C 标准保留
+ `(c)`: `_t` 结尾的标识符 C 标准保留
+ `(d)`: 当心与 C 标准定义的宏冲突
+ `(e)`: 双下划线 `__` 开头的标识符 c 标准保留
+ `(f)`: `E` 开头的标识符 C 标准保留


**题目 3**【代码风格】：以下函数声明有什么问题？

```c
double calc(size_t N, size_t I, double data[N][I]);
```

答案：

+ 函数名 `calc` 太宽泛，不知道计算的是什么内容
+ `N` 和 `I` 看起来像某个宏

**题目 4**【代码风格】：`char const *p` 和 `const char *p` 有区别吗？为什么教材推荐前者？

答案: 没区别，表示都是 `p` 指向的对象是只读对象。但是，前者更容易找到标识符的类型

**题目 5**【代码风格】：C23 为什么要求标识符中的 Unicode 字符必须通过 NFC 规范化？举一个字形相同但编码不同的例子。

答案：NFC 规范化后表示的同一个字符才能作为表示。例如，希腊大写 Alpha "Α" 和拉丁大写 "A" 看起来一样，但是编码不同

### 编程题

#### 练习 1：代码风格审查（15 分钟）【代码风格】

以下代码有多处风格问题，请找出并修复所有问题。

```c
#include <stdio.h>
#include <string.h>

#define MAX(a,b) (a)>(b)?(a):(b)

struct Student{
char Name[50];
int Age;
double Score;
};

typedef struct Student student_t;

void print_student(struct Student * s){
printf("Name: %s\n",s->Name);
printf("Age: %d\n",s->Age);
printf("Score: %f\n",s->Score);
}

int main(void){
struct Student students[3]={{"Alice",20,95.5},{"Bob",21,87.3},{"Charlie",22,91.0}};
int max_score=0;
for(int i=0;i<3;i++){
if(students[i].Score>students[max_score].Score){
max_score=i;
}
}
print_student(&students[max_score]);
return 0;
}
```

**要求**：
+ 修复所有格式化问题（大括号、空格、缩进、左绑定）
+ 修复所有命名问题（reserved 标识符、命名约定）
+ 修复宏定义问题
+ 编译验证：`gcc -Wall -Wextra -std=c23 -o style_fix style_fix.c`

答案

```c
#include <stdio.h>
#include <string.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))

typedef struct Student Student; // 不应该使用 _t 结尾的。最好前向声明

struct Student{
    char Name[50];
    int Age;
    double Score;
};

// typedef struct Student student_t;

void print_student(struct Student const* s){  // const 限制接口修改对象
    printf("Name: %s\n",s->Name);
    printf("Age: %d\n",s->Age);
    printf("Score: %f\n",s->Score);
}

int main(void){
    constexpr size_t size = 3;  // 不要使用幻数，使用命名常量
    struct Student students[size] = {
        {"Alice",20,95.5},
        {"Bob",21,87.3},
        {"Charlie",22,91.0}
    };

    int max_score = 0;
    for (int i=0; i < size; i++) {
        if(students[i].Score > students[max_score].Score){
            max_score=i;
        }
    }

    print_student(&students[max_score]);
    return 0;
}
```

---

#### 练习 2：命名重构（15 分钟）【代码风格】

以下函数和类型命名有问题，请按照"类型说概念、函数说动作、变量说状态"的原则重构。

```c
typedef struct {
    double data[100];
    int sz;
} arr;

void process(arr *a, double val);
double get(arr *a, int i);
void set(arr *a, int i, double val);
int check(arr *a);
void do_thing(arr *a, arr *b);
```

**要求**：
+ 为 `arr` 类型取一个表达概念的名字
+ 为每个函数取一个表达动作的名字
+ 考虑是否需要加项目前缀
+ 写出完整的头文件（含 include guard 和 doxygen 注释）

答案:

---

#### 练习 3：reserved 标识符检查（10 分钟）【代码风格】

编写一个简单的脚本（或手动检查），扫描以下代码中的 reserved 标识符冲突。

```c
// mylib.h
#ifndef MYLIB_H
#define MYLIB_H 1

typedef struct {
    char *str_data;
    int len;
    int errno_code;
} str_t;

void str_init(str_t *s, const char *input);
int str_compare(str_t *a, str_t *b);
char *str_cpy(str_t *s);
#define E_OK 0
#define E_FAIL -1

#endif
```

**要求**：
+ 列出所有违反 reserved 规则的标识符
+ 说明每条违反的具体原因
+ 给出修复后的版本

### 思考题

**思考题 1**【代码风格】：教材说"编码风格是文化问题"。你在自己的项目中（如 vblog、netdisk）使用的是什么风格？和 Modern C 教材的风格有什么差异？你觉得需要统一吗？

**思考题 2**【代码风格】：为什么 C 标准要保留 `str` 开头、`E` 开头、`_t` 结尾的标识符？如果不保留会怎样？

---

## 二、组织与文档

### 基础概念题（口头回答）

**题目 1**【组织与文档】：解释 what / what for / how / in which manner 四个层次的含义，并各举一个例子。

**题目 2**【组织与文档】：以下头文件有什么问题？

```c
// math_utils.h
double square(double x);
double cube(double x);
#define PI 3.14159
```

**题目 3**【组织与文档】：以下宏有什么问题？给出修复后的写法。

```c
#define MAX(a, b) a > b ? a : b
```

**题目 4**【组织与文档】：以下代码的输出是什么？为什么？

```c
#define SWAP(a, b) { int temp = a; a = b; b = temp; }

int main(void) {
    int x = 1, y = 2;
    if (x < y)
        SWAP(x, y);
    else
        x = 0;
    printf("x=%d, y=%d\n", x, y);
}
```

**题目 5**【组织与文档】：以下函数是纯函数吗？为什么？

```c
int counter = 0;

int next_id(int base) {
    ++counter;
    return base + counter;
}
```

**题目 6**【组织与文档】：解释 `[[nodiscard]]`、`[[fallthrough]]`、`[[noreturn]]` 三个属性的用途。

**题目 7**【组织与文档】：为什么在头文件中要使用 `[[__nodiscard__]]` 而不是 `[[nodiscard]]`？

### 编程题

#### 练习 1【组织与文档】：修复带陷阱的宏（15 分钟）

以下宏定义都有问题，请逐一修复并编写测试用例验证。

```c
/* 1. 求最大值 — 多次求值问题 */
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* 2. 最小值 — 缺少括号 */
#define MIN(a, b) (a < b ? a : b)

/* 3. 安全除法 — 悬垂 else */
#define SAFE_DIV(a, b, result) if ((b) != 0) *(result) = (a) / (b)

/* 4. 打印并返回 — 尾随分号问题 */
#define LOG_RETURN(val) printf("returning %d\n", val); return val
```

**要求**：

+ 用 `_Generic` 或 `do-while(false)` 等标准手法修复
+ 对每个宏编写至少 $2$ 个测试用例（包括边界情况）
+ 用 `static_assert` 或 `assert` 验证结果

---

#### 练习 2【组织与文档】：实现带文档的分数库（30 分钟）

实现一个分数（有理数）运算库，分离接口和实现。

**要求**：

+ 创建头文件 `rational.h`，包含 Include Guard、`struct rat` 类型定义、所有公开函数的声明、完整的 doxygen 文档（`@brief`、`@param`、`@return`）
+ 创建实现文件 `rational.c`，实现以下函数（所有运算函数必须是纯函数）
  + `rat rat_create(signed sign, size_t num, size_t denom)` — 构造
  + `rat rat_normalize(rat x)` — 约分
  + `rat rat_add(rat x, rat y)` — 加法
  + `rat rat_mul(rat x, rat y)` — 乘法
  + `rat rat_neg(rat x)` — 取反
  + `void rat_print(rat x)` — 打印（唯一非纯函数）
+ 创建 `main.c` 测试所有函数
+ 编译命令：`gcc -Wall -Wextra -std=c23 -o rational main.c rational.c`

**示例输出**：

```
1/2 + 1/3 = 5/6
1/2 * 2/3 = 1/3
-(1/2) = -1/2
```

---

#### 练习 3【组织与文档】：头文件组织（20 分钟）

将练习 8（位运算）的代码重构为头文件 + 实现文件的形式。

**要求**：

+ 创建 `bitops.h`：Include Guard、所有函数声明、doxygen 文档
+ 创建 `bitops.c`：所有函数实现、实现注释（解释算法思路，不陈述显而易见的）
+ 创建 `main.c`：测试所有函数
+ 编译命令：`gcc -Wall -Wextra -std=c23 -o bitops main.c bitops.c`

---

#### 练习 4【组织与文档】：属性练习（15 分钟）

编写程序使用 C23 属性。

**要求**：

+ 定义一个 `[[nodiscard]]` 函数 `safe_sqrt(double x)`，负数时返回 NaN
+ 定义一个 `[[deprecated("use safe_sqrt instead")]]` 函数 `my_sqrt(double x)`
+ 在 `switch` 中使用 `[[fallthrough]]`
+ 在一个未使用的参数上使用 `[[maybe_unused]]`
+ 使用双下划线形式 `[[__nodiscard__]]` 等

### 思考题

**思考题 1**【组织与文档】：为什么 C 语言选择用头文件（.h）+ 翻译单元（.c）的方式分离接口和实现，而不是像 Java 那样把接口和实现放在同一个文件中？这种设计有什么优缺点？

**思考题 2**【组织与文档】：`do-while(false)` 技巧虽然解决了悬垂 else 问题，但它本身很丑陋。有没有更好的替代方案？为什么 C 社区仍然接受它？

**思考题 3**【组织与文档】：纯函数要求"返回值只取决于参数"，但很多实际任务需要修改状态（如文件 IO、数据库操作）。纯函数在什么场景下仍然有价值？如何在需要副作用的场景中借鉴纯函数的设计思想？
