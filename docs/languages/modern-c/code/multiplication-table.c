/* multiplication-table.c - 打印 9×9 乘法表。*/
#include <stdio.h>

int main(void) {

    for (int i = 1; i < 10; ++i) {
        for (int j = 1; j <= i; ++j) {
            // j < 2 ? 3 : 4 用于指定乘积的最小宽度
            printf("%d * %d = %-*d", j, i,  j < 2 ? 3: 4, j * i);
        }
        printf("\n");
    }
    return 0;
}
