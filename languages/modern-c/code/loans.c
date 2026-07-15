/* loans.c - 判断申请人能否申请贷款 */
#include <stdio.h>

int main(void) {

    int age = {0};      // 年龄
    int income = {0};   // 年收入
    int balance = {0};  // 存款
    int loan = {0};     // 能够申请的额度

    printf("Enetr your age in years: ");
    scanf("%d", &age);
    printf("Enetr your annual income in dollars: ");
    scanf("%d", &income);
    printf("Enetr your current account balance in dollars: ");
    scanf("%d", &balance);

    /* 发放贷款的条件 */
    if (age >= 21 && (income >= 2'5000 || balance >= 100'000)) {
        // 能够申请的额度
        // + 2 倍的年收入小于存款的一半；发放 2 被年收入
        // + 2 倍的年收入大于或等于存款的一半；发放存款的一半
        loan = 2 * income < balance / 2 ? 2 * income : balance / 2;
        printf("You can borrow up to $: %d\n", loan);
    } else {
        printf("Unfortunately, you don't qualify for a loan.\n");
    }
    return 0;
}
