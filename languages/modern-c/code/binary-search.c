/* binary-search.c - 二分查找 */
#include <stdio.h>

int binary_search_iter(int array[static 1], int size, int target);
int binary_search_recursion(int array[static 1], int size, int target);

int main(void) {

    int size = {0};
    int target = {0};

    printf("输入数组大小: ");
    scanf("%d", &size);
    int array[size];

    printf("输入 %d 个已排序的整数: ", size);
    for (int i = 0; i < size; ++i) {
        scanf("%d", &array[i]);
    }

    printf("输入目标值: ");
    scanf("%d", &target);

    int index = binary_search_iter(array, size, target);
    if (index != -1) {
        printf("迭代法: 找到, 索引 = %d\n", index);
    }

    index = binary_search_recursion(array, size, target);
    if (index != -1) {
        printf("递归法: 找到, 索引 = %d\n", index);
    }

    if (index == -1) {
        printf("未找到\n");
    }

    return 0;
}

int binary_search_iter(int array[static 1], int size, int target) {
    int left = 0;
    int right = size - 1;
    while (left <= right)
    {
        int mid = (left + right) / 2;
        if (array[mid] == target) {
            return mid;
        } else if (array[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

int binary_search_recursion2(int array[static 1], int left, int right, int target) {
    if (left > right) {
        return -1;
    }
    int mid = (left + right) / 2;
    if (array[mid] == target) {
        return mid;
    } else if (array[mid] < target) {
        return binary_search_recursion2(array, mid+1, right, target);
    } else {
        return binary_search_recursion2(array, left, mid - 1, target);
    }
}

int binary_search_recursion(int array[static 1], int size, int target) {
    return binary_search_recursion2(array, 0, size-1, target);
}
