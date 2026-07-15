/* gcd.c - 计算两个数的最大公约数 */
#include <stdio.h>

int main(void) {
    int a = {0};
    int b = {0};
    printf("请输入两个正整数: ");
    scanf("%d%d", &a, &b);

    if (a < b) {
        int temp = a;
        a = b;
        b = temp;
    }

    while (b != 0)
    {
        int r = a % b;
        if (r == 0) {
            break;
        }
        a = b;
        b = r;
    }

    printf("最大公约数是: %d\n", b);
    return 0;
}
