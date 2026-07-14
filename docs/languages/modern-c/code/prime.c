/* prime.c - 判断一个数是否是素数 */
#include <stdio.h>

int main(void) {
    int number = {0};
    printf("请输入一个正整数：");
    scanf("%d", &number);

    if (number == 1) {
        printf("%d 不是素数\n", number);
        return 0;
    }

    bool is_prime = true;

    for (int i = 2; i * i <= number; ++i) {
        if (number % i == 0) {
            is_prime = false;
            break;
        }
    }

    if (is_prime) {
        printf("%d 是素数\n", number);
    } else {
        printf("%d 不是素数\n", number);
    }
    return 0;
}
