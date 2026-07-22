/* student.c - 2：学生成绩管理 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum {NAME_SIZE = 20, NO_SIZE = 20, STUDENT_SIZE = 5};

typedef struct student Student;

struct student {
    char name[NAME_SIZE];
    char no[NO_SIZE];
    double grade;
};

double st_grade_avg(Student studengs[static 1], int size);
Student* st_grade_max(Student studentS[static 1], int size);
Student* st_grade_min(Student studentS[static 1], int size);
void st_sort(Student students[static 1], int size);

int main(void) {
    Student students[STUDENT_SIZE] = {};

    printf("请输入 %d 个学生信息:\n", STUDENT_SIZE);
    for (size_t i = 0; i < STUDENT_SIZE; ++i) {
        printf("学生 %zu 姓名: ", i + 1);
        fgets(students[i].name, NAME_SIZE, stdin);
        printf("学生 %zu 学号: ", i + 1);
        fgets(students[i].no, NO_SIZE, stdin);
        printf("学生 %zu 成绩: ", i + 1);
        fscanf(stdin, "%lf%*c", &students[i].grade); // %*c 消耗换行符
    }

    double avg = st_grade_avg(students, STUDENT_SIZE);
    printf("平均分: %.2f\n", avg);
    Student* max = st_grade_max(students, STUDENT_SIZE);
    Student* min = st_grade_min(students, STUDENT_SIZE);
    printf("最高分: %.2f\n", max->grade);
    printf("最低分: %.2f\n", min->grade);

    st_sort(students, STUDENT_SIZE);
    printf("排序后:\n");
    for (size_t i = 0; i < STUDENT_SIZE; ++i) {
        printf("%zu. %s %.2f", i+1, students[i].name, students[i].grade);
    }
    puts("");

    return 0;
}

double st_grade_avg(Student studengs[static 1], int size) {
    double result = {0};
    for (size_t i = 0; i < size; ++i) {
        result += studengs[i].grade;
    }
    return result / size;
}

Student* st_grade_max(Student students[static 1], int size) {
    Student* max = &students[0];
    for (size_t i = 1; i < size; ++i) {
        if (max->grade < students[i].grade) {
            max = &students[i];
        }
    }
    return max;
}
Student* st_grade_min(Student students[static 1], int size) {
    Student* min = &students[0];
    for (size_t i = 1; i < size; ++i) {
        if (min->grade > students[i].grade) {
            min = &students[i];
        }
    }
    return min;
}

int compar(const void* s1, const void* s2) {
    return ((Student*)s1)->grade - ((Student*)s2)->grade;
}

void st_sort(Student students[static 1], int size) {
    qsort(students, size, sizeof(students[0]), compar);
}

