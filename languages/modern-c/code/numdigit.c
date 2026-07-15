/* numdigit.c - 计算整数的位数 */
#include <stdio.h>

int main(void) {

    int number = {};

    printf("Enetr a integer number: ");
    scanf("%d", &number);

    int digits = {};

    do {
        number /= 10;
        ++digits;
    } while (number != 0);

    printf("The number has %d digit(s).\n", digits);

    return 0;
}
