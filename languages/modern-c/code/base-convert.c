/* base-convert.c - 整数进制转换器 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* 十进制转任意进制字符串（2/8/16） */
void dec_to_base(unsigned int number, int base, char buf[static 65]) {
    if (number == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    char const digits[] = "0123456789abcdef";
    char tmp[65];
    int len = 0;

    while (number > 0) {
        tmp[len++] = digits[number % base];
        number /= base;
    }

    /* 反转 */
    for (int i = 0; i < len; ++i) {
        buf[i] = tmp[len - 1 - i];
    }
    buf[len] = '\0';
}

/* 任意进制字符串转十进制（2/8/16） */
unsigned int base_to_dec(char const str[static 1], int base) {
    unsigned int result = 0;
    for (size_t i = 0; str[i]; ++i) {
        int digit = 0;
        if (isdigit((unsigned char)str[i])) {
            digit = str[i] - '0';
        } else {
            digit = tolower((unsigned char)str[i]) - 'a' + 10;
        }
        result = result * base + digit;
    }
    return result;
}

int main(void) {
    char buf[65] = {};
    int number = 0;

    printf("输入一个十进制整数: ");
    scanf("%d", &number);

    unsigned int abs_val = (number < 0) ? -(unsigned int)number : (unsigned int)number;
    char const *sign = (number < 0) ? "-" : "";

    dec_to_base(abs_val, 2, buf);
    printf("%d -> 二进制: %s%s\n", number, sign, buf);

    dec_to_base(abs_val, 8, buf);
    printf("%d -> 八进制: %s%s\n", number, sign, buf);

    dec_to_base(abs_val, 16, buf);
    printf("%d -> 十六进制: %s%s\n", number, sign, buf);

    printf("\n二进制 11111111 -> 十进制: %u\n", base_to_dec("11111111", 2));
    printf("八进制 377 -> 十进制: %u\n", base_to_dec("377", 8));
    printf("十六进制 ff -> 十进制: %u\n", base_to_dec("ff", 16));

    return 0;
}
