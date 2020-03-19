#ifndef _READING_H_
#define _READING_H_


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "commands.h"
#include "avl.h"

int read_line(char **buf, int *buf_size, int *len);

int split_line(char *str, int len, char **words);

#endif
