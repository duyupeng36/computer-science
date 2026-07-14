/* prime2.c - 输出 2 ~ 100 中的所有素数 */
#include <stdio.h>

int main(void) {

    printf("%3d", 2);
    int count = 1;
    for (int number = 3; number <= 100; number += 2) {
        bool flag = true;
        for (int factor = 2; factor * factor <= number; ++factor) {
            if (number % factor == 0) {
                flag = false;
                break;
            }
        }
        if (flag) {
            printf("%3d", number);
            ++count;
        }
        if (count % 10 == 0) {
            printf("\n");
        }
    }
    return 0;
}
