/* rhombus.c - 打印菱形 */
#include <stdio.h>

int main(void) {

    int layer = {0};

    printf("输入菱形的层级(奇数)：");
    scanf("%d", &layer);
    if (!(layer & 0x1)) {
        fprintf(stderr, "请输入奇数，而不是偶数 %d\n", layer);
        return 1;
    }

    // 打印上半部分
    for (int i = 1; i <= layer / 2; ++i) {
        for (int j = 0; j < 2 * i - 1; ++j) {
            if (j == 0) {
                printf("%*c", layer - i, '*');
                continue;
            }
            printf("*");
        }
        printf("\n");
    }
    // 打印下半部分
    for (int i = layer / 2 + 1; i; --i) {
        for (int j = 0; j < 2 * i - 1; ++j) {
            if (j == 0) {
                printf("%*c", layer - i, '*');
                continue;
            }
            printf("*");
        }
        printf("\n");
    }

    return 0;
}
