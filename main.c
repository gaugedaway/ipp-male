#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "avl.h"
#include "input.h"
#include "commands.h"


const int INIT_BUFFER_SIZE = 1024;


int process_line(Tree *world, char **buf, int *buf_size) {
    int len;
    int eof = read_line(buf, buf_size, &len);

    if ((*buf)[0] == '#')
        return eof;

    char *words[4];
    int n = split_line(*buf, len, words);

    if (n == 0)
        return eof;

    if (n == -1 || eof) {
        fputs("ERROR\n", stderr);
        return eof;
    }

    if (strcmp(words[0], "ADD") == 0 && n >= 2)
        cmd_add(world, words + 1, n - 1);
    else if (strcmp(words[0], "DEL") == 0)
        cmd_del(world, words + 1, n - 1);
    else if (strcmp(words[0], "PRINT") == 0 && n <= 3)
        cmd_print(world, words + 1, n - 1);
    else if (strcmp(words[0], "CHECK") == 0 && n >= 2)
        cmd_check(world, words + 1, n - 1);
    else
        fputs("ERROR\n", stderr);

    return eof;
}


int main(void) {
    char *buf = malloc(INIT_BUFFER_SIZE);
    int buf_size = INIT_BUFFER_SIZE;

    Tree world = avl_create();

    while (!process_line(&world, &buf, &buf_size));

    free(buf);
    avl_free(&world);

    return 0;
}
