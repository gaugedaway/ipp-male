#include "commands.h"
#include "avl.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "readline.h"


#define WHITESPACE(c) ((c) == 32 || (c) == 9 || (c) == 11 || (c) == 12 || (c) == 13)
#define NAME_CHAR(c) ((unsigned char) (c) >= 33)


/*
 * Maximum number of words (chunks of characters satisfying
 * NAME_CHAR(c)) to be processed by split_line, before signalling an error
 */
const int MAX_NUM_WORDS = 4;


/*
 * Reads a single line into the buffer (char*) *buf
 * Sets *len to length of the line (without newline).
 * When needed, it dynamically reallocates the buffer with twice
 * its previous size, so that even in case of very long lines
 * it runs in O(nlogn), where n is the length of the line.
 * It returns 0 if the line was terminated by '\n' and 1
 * if it was terminated by End Of File.
 */
int read_line(char **buf, int *buf_size, int *len) {
    int pos = 0;
    char c = 0;

    while (1) {
        while (pos < *buf_size && (c = getchar()) != '\n' && c != EOF)
            (*buf)[pos++] = c;

        if(c == '\n' || c == EOF)
            break;

        if (pos == *buf_size) {
            *buf_size *= 2;
            *buf = realloc(*buf, *buf_size);
        }
    }

    (*buf)[pos] = '\0';
    *len = pos;
    return c == '\n' ? 0 : 1;
}


/*
 * Splits a single string 'str' into separate words (chunks of characters
 * satisfying NAME_CHAR(c)), separated by whitespace (characters satisfying
 * WHITESPACE(c)). 'len' should be length str (the unparsed string can contain
 * '\0' inside, so the actual length needs to be provided separately).
 * It doesn't actually copy the words, rather it places '\0' after them
 * and saves pointers to their beginning in the table 'words' (in other words
 * it uses the original string as a memory to store them).
 *
 * If the string contains characters that don't satisfy
 * either NAME_CHAR(c) or WHITESPACE(c), or if the number of words exceeds
 * MAX_NUM_WORDS, it returns -1. Otherwise it returns the number of words.
 */
int split_line(char *str, int len, char **words) {
    int n = 0;
    int pos = 0;

    while (WHITESPACE(str[pos]))
        pos++;

    while (n < MAX_NUM_WORDS && pos < len) {
        words[n++] = str + pos;
        while (NAME_CHAR(str[pos]))
            pos++;

        if(pos < len && !WHITESPACE(str[pos]))
            return -1;

        if(pos < len)
            str[pos++] = '\0';

        while (WHITESPACE(str[pos]))
            pos++;
    }

    if (pos < len)
        return -1;

    return n;
}