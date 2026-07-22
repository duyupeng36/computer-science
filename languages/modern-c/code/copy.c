/* copy.c - 实现 cp 指令 */
#include <stdio.h>

int main(int argc, char* argv[argc + 1]) {

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <src> <dst>\n", argv[0]);
        return 1;
    }

    FILE * src = fopen(argv[1], "rb");
    if (!src) {
        perror("fopen:");
        return 1;
    }
    FILE *dst = fopen(argv[2], "wb");
    if (!dst) {
        perror("fopen:");
        return 1;
    }
    int ch = {0};
    while ((ch = fgetc(src)) != EOF) {
        if (fputc(ch, dst) == EOF) {
            perror("fputc:");
            break;
        }
    }
    if (feof(src)) {
        return 0;
    } else {
        perror("fgetc:");
        return 1;
    }
}

