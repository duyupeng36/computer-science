/* square2.c - 改进的平方表计算方法 */
#include <stdio.h>

int main(void) {
    int number = {0};

    printf("Enetr a integer number: ");
    scanf("%d", &number);

    if (number < 0) {
        number = -number;
    }

    for (int i = 1, square = 1, odd = 1; i <= number; ++i,  odd += 2, square += odd) {
        printf("%10d%10d\n", i, square);
    }

    return 0;
}
