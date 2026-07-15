/* square.c - 显示平方表 */
#include <stdio.h>

int main(void) {
    int number = {};

    printf("Enetr number of entries in table: ");
    scanf("%d", &number);

    int i = 1;
    while (i <= number) {
        printf("%10d%10d\n", i, i * i);
        ++i;
    }

    return 0;
}
