#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "avl.h"
#include "input.h"
#include "commands.h"


const int INIT_BUFFER_SIZE = 1024;


int process_line(Tree *world, char **buf, int *buf_size) {
    int len;
    int read_line_result = read_line(buf, buf_size, &len);

    if (read_line_result == -1)
        return -1;

    if ((*buf)[0] == '#')
        return read_line_result;

    char *words[4];
    int n = split_line(*buf, len, words);

    if (n == 0)
        return read_line_result;

    if (n == -1 || read_line_result == 1) {
        fputs("ERROR\n", stderr);
        return read_line_result;
    }

    int cmd_result = 0;
    if (strcmp(words[0], "ADD") == 0 && n >= 2)
        cmd_result = cmd_add(world, words + 1, n - 1);
    else if (strcmp(words[0], "DEL") == 0)
        cmd_del(world, words + 1, n - 1);
    else if (strcmp(words[0], "PRINT") == 0 && n <= 3)
        cmd_print(world, words + 1, n - 1);
    else if (strcmp(words[0], "CHECK") == 0 && n >= 2)
        cmd_check(world, words + 1, n - 1);
    else
        fputs("ERROR\n", stderr);

    if (cmd_result == -1)
        return -1;

    return read_line_result;
}


int main(void) {
    int buf_size = INIT_BUFFER_SIZE;
    char *buf = malloc(INIT_BUFFER_SIZE);

    if(!buf)
        return 1;

    Tree world = avl_create();

    int result;
    while (!(result = process_line(&world, &buf, &buf_size)));

    free(buf);
    avl_free(&world);

    if(result == -1)
        return 1;

    return 0;
}
