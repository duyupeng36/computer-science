/* atoi.c - 实现 atoi 将字符串转换为 int 类型的值 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum { BUFF_SIZE = 100 };

bool atoi(char const str[static 1], int* result);

int main(void) {
    char str[BUFF_SIZE];
    printf("输入: ");

    if (!fgets(str, BUFF_SIZE, stdin)) {
        perror("fgets:");
        return 0;
    }

    int result = {0};
    if (atoi(str, &result)) {
        printf("输出: %d\n", result);
    } else {
        fprintf(stderr, "输出: 溢出\n");
    }

    return 0;
}


bool atoi(char const str[static 1],  int* result) {
    *result = 0;
    if (!strlen(str)) {
        return true;
    }
    size_t start = strspn(str, " \n\f\r\v\t");

    // 处理正负号: 默认是正数
    int sign = 1;
    if (str[start] == '-') {
        sign *= -1;
        ++start;
    } else if (str[start] == '+') {
        ++start;
    }

    int base = 1;
    size_t end = start;
    while (str[end] && isdigit(str[end])) {
        ++end;
    };

    if (end == start) {
        *result = 0;
        return true;
    }

    // i >= start 可能会始终成立
    for (size_t i = end - 1; ; --i) {
        if (sign > 0) {
            *result += (str[i] - '0') * base;
        } else {
            *result -= (str[i] - '0') * base;
        }

        base *= 10;
        // 判断溢出
        if ((sign > 0 && *result < 0) || (sign < 0 && *result > 0)) {
            return false;
        }
        // 判断是否继续
        if (i == start) {
            break;
        }
    }
    return true;
}
