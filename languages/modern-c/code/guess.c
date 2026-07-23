/* guess.c - 猜数字 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(void) {
    time_t t = time(nullptr);
    srand((unsigned int)t);
    int number = rand() % 100 + 1;  // 随机生成一个数字 1 ~ 100 之间的数字
    int guss = {0};
    size_t count = 1;

    printf("我想了一个 1~100 的数字，请猜：");
    scanf("%d", &guss);
    while (guss != number) {
        if (guss < number) {
            printf("小了！再猜：");
            scanf("%d", &guss);
        } else if (guss > number) {
            printf("大了！再猜：");
            scanf("%d", &guss);
        } else {
            break;
        }
        ++count;
    }

    printf("恭喜你，猜对了！共猜了 %zu 次\n", count);
    return 0;
}
