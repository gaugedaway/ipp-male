/*
 * Author: Adam Bac
 * File: main.c
 *
 * Initialisation and the main loop of the program.
 */

#include "avl.h"
#include "commands.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initial size of the buffer to which the lines are being read.
const int INIT_BUFFER_SIZE = 1024;

/*
 * Processes one line of the input. Arguments: a pointer to
 * the AVL tree representing the world, a pointer to a buffer
 * to which the lines are being read and a pointer to the size
 * of that buffer.
 * Returns 1 if it encounters End Of File, -1 when it can't allocate
 * needed memory and 0 otherwise.
 */
int process_line(Tree *world, char **buf, int *buf_size) {
    int length;
    int read_line_result = read_line(buf, buf_size, &length);

    // check for memory allocation problems
    if (read_line_result == -1)
        return -1;

    // check if the line is a comment
    if ((*buf)[0] == '#')
        return read_line_result;

    char *words[4];
    int num_words = split_line(*buf, length, words);

    // if the line empty return 1 when it contains EOF, 0 otherwise
    if (num_words == 0)
        return read_line_result;

    // check if the line is improperly formatted
    // or not empty but containing EOF
    if (num_words == -1 || read_line_result == 1) {
        fputs("ERROR\n", stderr);
        return read_line_result;
    }

    int cmd_result = 0;
    if (strcmp(words[0], "ADD") == 0 && num_words >= 2)
        cmd_result = cmd_add(world, words + 1, num_words - 1);
    else if (strcmp(words[0], "DEL") == 0)
        cmd_del(world, words + 1, num_words - 1);
    else if (strcmp(words[0], "PRINT") == 0 && num_words <= 3)
        cmd_print(world, words + 1, num_words - 1);
    else if (strcmp(words[0], "CHECK") == 0 && num_words >= 2)
        cmd_check(world, words + 1, num_words - 1);
    else
        fputs("ERROR\n", stderr);

    // check if the command failed because of allocation problems
    if (cmd_result == -1)
        return -1;

    // return whether the line contains EOF
    return read_line_result;
}

int main(void) {
    int buf_size = INIT_BUFFER_SIZE;
    char *buf = malloc(INIT_BUFFER_SIZE);
    if (!buf)
        return 1;

    Tree world = avl_create();

    // read lines until EOF is encountered or there's a problem
    // with allocating needed memory
    int result;
    while (!(result = process_line(&world, &buf, &buf_size)));

    free(buf);
    avl_free(&world);

    // check if allocation problems occured
    if (result == -1)
        return 1;

    return 0;
}
