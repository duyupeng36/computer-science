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
