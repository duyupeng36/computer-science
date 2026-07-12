/* bad.c - 演示了一些 C 程序的缺陷 */

/* 程序主要做的事情 */
void main() {  // (1)!
    // 声明
    int i;
    double A[5] = {
        9.0,
        2.9,
        3.E+25,
        .00007,
    };

    // 做一些工作
    for (i = 0; i < 5; ++i) {
        printf("element %d is %g, \tits square is %g\n",
               i,
               A[i],
               A[i] * A[i]);
    }
    return 0;
}
