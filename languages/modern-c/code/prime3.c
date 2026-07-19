/* prime3.c - 素数判定函数 */
#include <stdio.h>

// 声明 is_prime 函数：告诉编译器这里有一个函数，接受一个 int 类型的参数，并返回一个 bool 类型的值
bool is_prime(int number);


int main(void) {
    int number = {0};
    printf("请输入一个正整数：");
    scanf("%d", &number);

    if (is_prime(number)) {
        printf("%d 是素数\n", number);
    } else {
        printf("%d 不是素数\n", number);
    }
    return 0;
}

// 定义 is_prime 函数: 告诉编译器函数的具体实现。注意：必须和函数声明保持原型一致
bool is_prime(int number) {
    if (number < 0) {
        number = -number;
    }

    // 2 是已知素数
    if (number == 2) {
        return true;
    }

    // 0、除 2 以外的其他偶数都不是素数
    if ((number == 0) || ((number != 2) && (number % 2 == 0))) {
        return false;
    }

    for (int i = 3; i * i <= number; i += 2) {
        // 只要找到除了 1 和 number 以外的能够整除 number 的因子，number 就不是素数
        if (number % i == 0) {
            return false;
        }
    }

    return true;
}
