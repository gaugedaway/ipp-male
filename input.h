/*
 * Author: Adam Bac
 * File: input.h
 *
 * Implements reading lines of arbitrary size from stdin
 * and parsing raw input lines.
 */


#ifndef _READING_H_
#define _READING_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int read_line(char **buf, int *buf_size, int *length);

int split_line(char *str, int length, char **words);


#endif
