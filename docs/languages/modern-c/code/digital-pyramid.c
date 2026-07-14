/* digital-pyramid.c - 数字金字塔 */
#include <stdio.h>

int main(void) {

    int layer = {0};
    printf("输入一个正整数: ");
    scanf("%d", &layer);
    if (layer < 0) {
        fprintf(stderr, "请输入正整数");
        return 1;
    }

    for (int i = 1; i <= layer; ++i) {
        for (int j = 0; j < i; j++) {
            if (j == 0) {
                printf("%*d", layer - i + 1, i);
                continue;
            }
            printf(" %d", i);
        }
        printf("\n");
    }

    return 0;
}
