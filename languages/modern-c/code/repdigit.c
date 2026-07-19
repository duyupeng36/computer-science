/* repdigit.c - 检查数字中是否出现重复数字 */
#include <stdio.h>

constexpr size_t BASE = 10;

int main(void) {
    int number = {};
    size_t digits[BASE] = {};

    printf("Enter a number: ");
    scanf("%d", &number);

    do {
        ++digits[number % BASE];
        number /= BASE;
    } while (number != 0);

    printf("Repeated digit:");
    for (size_t i = 0; i < BASE; ++i) {
        if (digits[i] > 1) {
            printf(" %zu", i);
        }
    }
    printf("\n");

    return 0;
}
