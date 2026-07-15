/* broker.c - 计算股票经纪人的佣金 */
#include <stdio.h>

int main(void) {

    double commission = {}; // 佣金
    double value = {};      // 交易金额

    printf("Enter value of trade: ");
    scanf("%lf", &value);

    if (value < 2'500.0) {
        commission = 30 + 0.017 * value;
    } else if(value < 6'250.0) {
        commission = 56 + 0.0066 * value;
    } else if (value < 20'000.0) {
        commission = 76 + 0.0034 * value;
    } else if (value < 50'000.0) {
        commission = 100 + 0.0022 * value;
    } else if (value < 500'000.0) {
        commission = 155 * 0.0011 * value;
    } else {
        commission = 255 * 0.0009 * value;
    }

    if (commission < 39.0) {
        commission = 39.0;
    }

    printf("Commission: %.2lf\n", commission);
    return 0;
}
