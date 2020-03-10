#include <stdlib.h>
#include <stdio.h>
#include "readline.h"


int read_line(char **buf, int *buf_size) {
    int pos = 0;

    while(1) {
        while(pos < *buf_size && ((*buf)[pos++] = getchar()) != '\n');
        if((*buf)[pos - 1] == '\n') break;
        if(pos == *buf_size) {
            *buf_size *= 2;
            *buf = realloc(*buf, *buf_size);
        }
    }

    (*buf)[pos - 1] = '\0';
    return pos - 1;
}
