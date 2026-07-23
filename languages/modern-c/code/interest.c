/* interest.c - 显示投资价值 */
#include <stdio.h>

// 计算数组的长度
#define SIZE(a) (sizeof(a) / sizeof((a)[0]))

constexpr double BALANCE = 100.00; // 存款

int main(void) {

    int rate = {};      // 利率。按照 100.00 进行缩放。真实利率时 rate / 100
    int years = {};     // 投资年限

    constexpr size_t size = 5;
    double value[size];

    printf("Enter interest rate: ");
    scanf("%d", &rate);
    printf("Enter number of years: ");
    scanf("%d", &years);

    printf("\nYears");
    for (size_t i = 0; i < SIZE(value); ++i) {
        printf("%6zu%%", rate + i);
        value[i] = BALANCE;
    }
    printf("\n");

    // 计算每年的投资价值
    for (int year = 1; year <= years; ++year) {
        printf("%3d   ", year);
        for (size_t i = 0; i < SIZE(value); ++i) {
            // 年复利
            value[i] += (rate + i) / 100.0 * value[i];
            printf("%7.2f", value[i]);
        }
        printf("\n");
    }

    return 0;
}
