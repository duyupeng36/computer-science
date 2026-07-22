/* bitop.c - 位操作 */
#include <stdio.h>


bool is_power_of_two(int number) {
    if (number < 0) {
        number = -number;
    }
    return !(number & (number - 1));
}

size_t one_bit_count(unsigned int number) {
    size_t count = 0;
    while (number > 0)
    {
        (void)(number & 0x1 ? ++count : 0);
        number >>= 1;
    }
    return count;
}

void swap(int *a, int *b) {
    *a ^= *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

unsigned set_bit(unsigned a, int n) {
    return a | ((unsigned)1 << n);
}

unsigned clear_bit(unsigned a, int n) {
    return a & (!(unsigned)1 << n);
}

