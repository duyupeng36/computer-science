/* fibonacci.c - 斐波拉契数列 */
#include <stdio.h>

int main(void) {

    constexpr int count = 20;

    for (int i = 1, a = 1, b = 1; i <= count; ++i) {
        printf("%d ", a);
        int temp = a;
        a = b;
        b = temp + a;

        if (i % 5 == 0) {
            printf("\n");
        }
    }
    return 0;
}
