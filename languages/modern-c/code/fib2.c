/* fib2.c - 斐波拉契数列(缓存递归) */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

size_t fib(size_t n, size_t cache[static (n+1)]);

int main(int argc, char* argv[argc + 1]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return 1;
    }

    size_t number = strtoull(argv[1], nullptr, 10);

    size_t cache[number+1];

    for (size_t i = 0; i < number + 1; ++i) {
        cache[i] = 0;
    }
    // 初始值
    cache[1] = 1;
    cache[2] = 1;

    size_t result = fib(number, cache);

    printf("Fib n item: %zu\n", result);

    return 0;
}

// 前置条件：cache 数组被提取初始化为 {0, 1, 1, 0, 0, ...}
size_t fib(size_t n, size_t cache[static (n+1)]) {
    assert(n != 0);
    // 第 n 项没有被计算，去计算
    if (!cache[n]) {
        cache[n] = fib(n-1, cache) + fib(n-2, cache);
    }
    // 第 n 项已经计算过了，直接返回
    return cache[n];
}
