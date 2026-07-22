/* bubble-sort.c - 冒泡排序 */
#include <stdio.h>

void bubble_sort(int array[static 1], int szie);

int main(void) {
    int size = {0};
    printf("输入数组大小: ");
    scanf("%d%*c", &size);

    int array[size];
    printf("输入 %d 个整数: ", size);
    for (int i = 0; i < size; ++i) {
        scanf("%d", &array[i]);
    }

    printf("排序前:");
    for (int i = 0; i < size; ++i) {
        printf(" %d", array[i]);
    }
    puts("");
    bubble_sort(array, size);
    printf("排序后:");
    for (int i = 0; i < size; ++i) {
        printf(" %d", array[i]);
    }
    puts("");

    return 0;
}

void bubble_sort(int array[static 1], int size) {
    for (int i = 0; i < size; ++i) {
        bool exchanged = false;
        for (int j = 0; j < size - i - 1; ++j) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
                exchanged = true;
            }
        }
        if (!exchanged) {
            break;
        }
    }
}
