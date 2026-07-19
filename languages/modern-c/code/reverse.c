/* reverse.c  - 数组反序*/
#include <stdio.h>

constexpr int size = 10;
int main(void) {
    int a[size];

    printf("Enter %d numbers: ", size);
    for (int i = 0; i < size; ++i) {
        scanf("%d", &a[i]);
    }

    printf("In reverse order:");
    for (int i = size - 1; i >= 0; --i) {
        printf(" %d", a[i]);
    }
    printf("\n");

    return 0;
}
