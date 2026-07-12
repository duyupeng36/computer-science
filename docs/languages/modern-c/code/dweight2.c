/* dweight.c - 计算箱子的空间重量 */

#include <stdio.h>

int main(int argc, [[maybe_unused]]char* argv[argc + 1]) {

    int height;  // 箱子的高度
    int length;  // 箱子的长度
    int width;   // 箱子的宽度
    int volume;  // 箱子的体积
    int weight;  // 箱子的重量

    printf("Enter height of box: ");
    scanf("%d", &height);
    printf("Enter length of box: ");
    scanf("%d", &length);
    printf("Enter width of box: ");
    scanf("%d", &width);

    volume = height * length * width;
    weight = (volume + 165) / 166;

    printf("Dimensions: %d x %d x %d\n", length, width, height);
    printf("Volume (cubic inches): %d\n", volume);
    printf("Dimensional weight (pounds): %d\n", weight);

    return 0;
}
