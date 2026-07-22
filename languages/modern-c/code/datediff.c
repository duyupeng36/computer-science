/* datediff.c - 计算两个日期相差的天数 */
#include <stdio.h>
#include <time.h>
#include <tgmath.h>

#define ONE_DAY_SEC (24 * 3600)

int main(void) {

    int year = 0;
    int month = 0;
    int day = 0;

    printf("请输入第一个日期（年 月 日）：");
    scanf("%d%d%d", &year, &month, &day);
    struct tm local1 = {
        .tm_mon=month-1,
        .tm_mday=day-1,
        .tm_year=year-1900
    };

    printf("请输入第二个日期（年 月 日）：");
    scanf("%d%d%d", &year, &month, &day);
    struct tm local2 = {
        .tm_mon=month-1,
        .tm_mday=day-1,
        .tm_year=year-1900
    };

    time_t t1 = mktime(&local1);
    time_t t2 = mktime(&local2);

    double diff = fabs(difftime(t1, t2));  // 两个日期相差多少秒

    double days = diff / ONE_DAY_SEC;

    printf("两个日期相差 %.0f\n", days);

    return 0;
}
