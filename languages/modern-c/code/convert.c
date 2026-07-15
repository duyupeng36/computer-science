/* convert.c - 实现摄氏度与华氏度的互相转换。*/
#include <stdio.h>

int main(void) {
    double temperature = {0.0};
    char ch = {0};
    printf("请输入温度值和单位（如 36.5 C）：");
    scanf("%lf", &temperature);
    do {
        ch = getchar();
        if (ch == 'f' || ch == 'F') {
            break;
        }
        if (ch == 'c' || ch == 'C') {
            break;
        }
    } while (true);

    double result = {0};

    switch (ch)
    {
    case 'f':
    case 'F':
        /* 华氏度 -> 摄氏度 */
        /* C = 5(F - 32) / 9 */
        result = 5.0 * (temperature - 32.0) / 9.0;
        printf("转换结果：%.2f C\n", result);
        break;

    case 'c':
    case 'C':
        /* 摄氏度 -> 华氏度 */
        /* F = C * 9/5 + 32 */
        result = temperature * 9.0 / 5.0 + 32.0;
        printf("转换结果：%.2f F\n", result);
        break;
    default:
        printf("非法的温度单位: 只允许 F(f) 或 C(c)");
        break;
    }
    return 0;
}
