/* celius.c - 华氏温度转摄氏温度 */

#include <stdio.h>

#define FREEZING_PT 32.0
#define SCALE_FACTOR (5.0 / 9.0)

int main(void) {
    double fahrenheit, celsius;

    printf("Enetr Fahrenheit temperature: ");
    scanf("%lf", &fahrenheit);
    celsius = (fahrenheit - FREEZING_PT) * SCALE_FACTOR;
    printf("Celeius equivalent: %.2f\n", celsius);
    return 0;
}
