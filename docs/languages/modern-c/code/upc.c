#include <stdio.h>

int main(void) {
    size_t digits[11];

    printf("请输入 11 位数字: ");
    for (size_t i = 0; i < 11; ++i) {
        scanf("%1zu", &digits[i]);
    }

    /* 第一步：奇数位求和 */
    size_t odd_sum = 0;
    for (size_t i = 0; i < 11; i += 2) {
        odd_sum = odd_sum + digits[i];
    }

    /* 第二步：偶数位求和 */
    size_t even_sum = 0;
    for (size_t i = 1; i < 11; i += 2) {
        even_sum = even_sum + digits[i];
    }

    /* 校验位：9 - (odd_sum * 3 + even_sum - 1) % 10 */
    size_t check_digit = 9 - (odd_sum * 3 + even_sum - 1) % 10;

    printf("校验位: %zu\n", check_digit);

    return 0;
}
