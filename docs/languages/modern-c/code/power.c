/* power.c - 快速幂 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[argc + 1]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <base> <exp>", argv[0]);
        return EXIT_FAILURE;
    }

    double base = strtod(argv[1], nullptr);
    int exp = (int)strtol(argv[2], nullptr, 10);

    if (exp < 0) {
        base = 1 / base;
        exp = -exp;
    }

    double result = 1.0;

    while (exp != 0) {
        if ((exp & 0x1) == 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }

    printf("Result is: %.2g\n", result);

    return EXIT_SUCCESS;
}
