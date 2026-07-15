#include <stdio.h>

int main() {

    signed int      d = -0x80000000;    // 危险: 同样危险！-0x80000000 还是 0x80000000
    printf("%#x\n", d);
    return 0;
}
