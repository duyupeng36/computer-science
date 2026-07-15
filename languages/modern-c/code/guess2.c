/* guess2.c - 猜数字改进版：用户最多猜测 7 次  */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(void) {
    time_t t = time(nullptr);
    srand((unsigned int)t);
    int number = rand() % 100 + 1;  // 随机生成一个数字 1 ~ 100 之间的数字
    int guess = {0};

    constexpr size_t count = 7;

    printf("我想了一个 1~100 的数字，你有 7 次机会！\n");
    bool guessed = false;
    for (size_t i = 0; i < count; ++i) {
        printf("第 %zu 次猜测：", i + 1);
        scanf("%d", &guess);
        if (guess > number) {
            printf("大了\n");
        } else if (guess < number) {
            printf("小了\n");
        } else {
            guessed = true;
            printf("恭喜你，猜对了！共猜了 %zu 次", i + 1);
            break;
        }
    }
    if (!guessed) {
        printf("抱歉，猜错了！正确值是 %d\n", number);
    }
    return 0;
}
