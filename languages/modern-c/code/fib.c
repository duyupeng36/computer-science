/* fib.c - 斐波拉契数列 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

size_t fib(size_t n);

int main(int argc, char* argv[argc + 1]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return 1;
    }

    size_t number = strtoull(argv[1], nullptr, 10);

    size_t result = fib(number);

    printf("Fib n item: %zu\n", result);

    return 0;
}

size_t fib(size_t n) {
    assert(n != 0);
    if (n < 3) {
        return 1;
    } else {
        return fib(n - 1) + fib(n - 2);
    }
}
