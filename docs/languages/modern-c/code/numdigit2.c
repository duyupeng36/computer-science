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
