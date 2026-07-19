/* gcd2.c - 使用递归实现欧几里得算法 */
#include <stdio.h>
#include <assert.h>

// 递归核心：假设 a <= b
inline size_t gcd2(size_t a, size_t b) {
    assert(a <= b);     // 前置条件检查
    if (!a) {
        return b;  // 终止条件 a == 0
    }
    size_t r = b % a;   // 递推公式 r 一定小于 a
    return gcd2(r, a);  // 递归调用
}

inline size_t gcd(size_t a, size_t b) {
    assert(a);      // 前置条件检查 a != 0
    assert(b);      // 前置条件检查 b != 0
    if (a <= b) {
        return gcd2(a, b);
    } else {
        return gcd2(b, a);
    }
}

int main(void) {
    size_t a = {0};
    size_t b = {0};
    printf("请输入两个正整数: ");
    scanf("%zu%zu", &a, &b);

    size_t result = gcd(a, b);
    printf("最大公约数是: %zu\n", result);

    return 0;
}
