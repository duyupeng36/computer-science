/* strings.c - 实现 str 相关的函数
- `my_strlen` — 计算字符串长度
- `my_strcpy` — 复制字符串
- `my_strcat` — 拼接字符串
- `my_strcmp` — 比较两个字符串
- `my_strchr` — 查找字符首次出现的位置
*/

#include <stddef.h>

size_t my_strlen(char const str[static 1]) {
    size_t len = 0;
    for (; str[len]; ++len);
    return len;
}

char* my_strcpy(char dst[static 1], char const src[static 1], size_t n) {

    size_t i = 0;
    for (i = 0; i < n-1 && src[i]; ++i) {
        dst[i] = src[i];
    }
    dst[i] = '\0';
    return dst;
}

char* my_strcat(char dst[static 1], char const src[static 1], size_t n) {
    size_t start = my_strlen(dst);
    size_t end = start;
    for (size_t i = 0; end < start + n - 1 && src[i]; ++end,++i) {
        dst[end] = src[i];
    }
    return dst;
}

int my_strcmp(char const str1[static 1], char const  str2[static 1]) {
    size_t i = 0;
    int result = 0;
    for (i = 0; str1[i] && str2[i]; ++i) {
        result = str1[i] - str2[i];
        if (result != 0) {
            return result;
        }
    }

    return my_strlen(str1) - my_strlen(str2);
}

int my_strchr(char const str[static 1], int ch) {
    int result = 0;
    for (result = 0; str[result]; ++result) {
        if (str[result] == ch) {
            return result;
        }
    }
    return -1;
}
