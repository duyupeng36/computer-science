/* grade.c - 成绩等级 */
#include <stdio.h>

int main(void) {
    int grade = {0};
    printf("请输入成绩：");
    scanf("%d", &grade);

    switch (grade / 10) {
    case 10:
        [[fallthrough]];
    case 9:
        printf("等级: A\n");
        break;
    case 8:
        printf("等级: B\n");
        break;
    case 7:
        printf("等级: C\n");
        break;
    case 6:
        printf("等级: D\n");
        break;

    default:
        printf("等级: F\n");
        break;
    }
}
