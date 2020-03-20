/*
 * Author: Adam Bac
 * File: input.h
 *
 * Implements reading lines of arbitrary size from stdin
 * and parsing raw input lines.
 */

#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_line(char **buf, int *buf_size, int *length);

int split_line(char *str, int length, char **words);

#endif
