/* leap-year.c - 判断闰年 */
#include <stdio.h>

int main(void) {
    int year = {0};
    printf("请输入年份：");
    scanf("%d", &year);

    if ((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0))) {
        printf("%d 年是闰年\n", year);
    } else {
        printf("%d 年不是闰年\n", year);
    }
    return 0;
}
