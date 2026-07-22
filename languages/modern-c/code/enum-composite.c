/* enum-composite.c - 枚举与复合字面量 */
#define  _GNU_SOURCE 1
#include <stdio.h>
#include <math.h>

enum Shape {CIRCULAR, RECTANGLE, TRIANGLE} ;


typedef struct {
    enum Shape type;
    union {
        struct  {
            double radius;
        } circular;
        struct {
            double width;
            double height;
        } rectangle;
         struct  {
            double base;
            double height;
        } triangle;
    } shape;
} Shape;


double area(Shape shape) {
    switch (shape.type)
    {
    case CIRCULAR:
        return M_PI * shape.shape.circular.radius * shape.shape.circular.radius;
    case RECTANGLE:
        return shape.shape.rectangle.width * shape.shape.rectangle.height;

    case TRIANGLE:

        return (shape.shape.triangle.base * shape.shape.triangle.height) / 2;
    }
    return 0;
}

int main(void) {
    printf("圆形（半径=5.0）面积: %.2f\n", area((Shape){.type=CIRCULAR, .shape={.circular.radius=5.0}}));
    printf("矩形（宽=4.0，高=6.0）面积: %.2f\n", area((Shape){.type=RECTANGLE, .shape={.rectangle.height=6.0, .rectangle.width=4.0}}));
    printf("三角形（底=3.0，高=8.0）面积: %.2f\n", area((Shape){.type=TRIANGLE, .shape={.triangle.base=3.0, .triangle.height=8.0}}));

    return 0;
}
