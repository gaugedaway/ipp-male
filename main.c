#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "avl.h"
#include "readline.h"


#define WHITESPACE(c) ((c) == 32 || (c) == 9 || (c) == 11 || (c) == 12 || (c) == 13)
#define NAME_CHAR(c) ((c) >= 33)


const int MAX_NUM_WORDS = 4;
const int INIT_BUFFER_SIZE = 1024;


char *buf;
int buf_size;

AVLTree world;


int split(char *str, char **words) {
    int n = 0;
    int pos = 0;
    while(n < MAX_NUM_WORDS && str[pos] != '\0') {
        while(WHITESPACE(str[pos])) pos++;
        if(str[pos] == '\0') break;

        words[n++] = str + pos;
        while(NAME_CHAR(str[pos])) pos++;
        if(str[pos] == '\0') break;
        str[pos++] = '\0';
    }

    while(WHITESPACE(str[pos])) pos++;
    if(str[pos] != '\0') return -1;
    return n;
}


void cmd_add(char **args, int n) {
    AVLTree *forest = avl_insert(&world, args[1]);
    if(n >= 3) {
        AVLTree *tree = avl_insert(forest, args[2]);
        if(n == 4) {
            avl_insert(tree, args[3]);
        }
    }
    puts("OK\n");
}


void cmd_del(char **args, int n) {
    if(n == 1) {
	avl_free(&world);
    }
    else if(n == 2) {
	avl_delete(&world, args[1]);
    }
    else {
	AVLTree *forest = avl_get_val(world, args[1]);
	if(!forest) return;
	if(n == 3) {
	    avl_delete(forest, args[2]);
	}
	else {
	    AVLTree *tree = avl_get_val(*forest, args[2]);
	    if(!tree) return;
	    avl_delete(tree, args[3]);
	}
    }
}


void cmd_print(char **args, int n) {
    if(n == 1) {
	avl_print(world, 0);
    }
    else {
	AVLTree *forest = avl_get_val(world, args[1]);
	if(!forest) return;
	if(n == 2) {
	    avl_print(*forest, 0);
	}
	else {
	    AVLTree *tree = avl_get_val(*forest, args[2]);
	    if(!tree) return;
	    avl_print(*tree, 0);
	}
    }
}


void process_line(void) {
    read_line(&buf, &buf_size);

    if(buf[0] == '#') return;

    char *args[4];
    int n = split(buf, args);

    if(n == -1) {
        puts("ERROR\n");
        return;
    }

    if(n == 0) return;

    if(strcmp(args[0], "ADD") == 0 && n >= 2) cmd_add(args, n);
    else if(strcmp(args[0], "DEL") == 0) cmd_del(args, n);
    else if(strcmp(args[0], "PRINT") == 0 && n <= 3) cmd_print(args, n);
    // else if(strcmp(args[0], "CHECK") == 0 && n >= 2) cmd_check(args, n);
    else {
        puts("ERROR\n");
        return;
    }
}


int main(void) {
    buf = malloc(INIT_BUFFER_SIZE);
    buf_size = INIT_BUFFER_SIZE;

    world = avl_create();

    while(1) process_line();

    free(buf);
    avl_free(&world);
}
