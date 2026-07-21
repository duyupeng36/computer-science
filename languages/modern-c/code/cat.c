/* cat.c - 将文件内容写入标准输出 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

enum {BUFF_SIZE = 32};


int main(int argc, char* argv[argc + 1]) {
    if (argc < 1) {
        fprintf(stderr, "Usage: %s <filename> [<filename>...]", argv[0]);
        return EXIT_FAILURE;
    }
    int ret = EXIT_FAILURE;
    char buffer[BUFF_SIZE] = {};
    for (int i = 1; i < argc; ++i) {
        FILE* instream = fopen(argv[i], "r");
        if (instream) {
            while (fgets(buffer, BUFF_SIZE, instream)) {
                fputs(buffer, stdout);
            }
            fclose(instream);
            ret = EXIT_SUCCESS;
        } else {
            fprintf(stderr, "Could not open %s: ", argv[i]);
            perror(0);
            errno = 0;
        }
    }
    return ret;
}
