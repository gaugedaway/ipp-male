/*
 * File: commands.c
 * Author: Adam Bac
 *
 * Implements executing four commands:
 * ADD, DEL, PRINT and CHECK.
 */


#include <string.h>
#include <stdio.h>
#include "avl.h"
#include "commands.h"


/*
 * All four functions take three arguments:
 * a pointer to the tree representing the world,
 * a table with arguments to the command (without the command itself)
 * and the number of arguments
 *
 * [cmd_add] returns -1 when it can't allocate needed memory,
 * 0 otherwise. The other functions don't return anthing.
 */

int cmd_add(Tree *world, char **args, int num_args) {
    Tree *level = world;
    for (int i = 0; i < num_args; i++)
        if (avl_insert(level, args[i], &level) == -1)
            return -1;

    puts("OK");
    return 0;
}


void cmd_del(Tree *world, char **args, int num_args) {
    if (num_args == 0)
        avl_free(world);

    Tree *level = world;
    for (int i = 0; i < num_args - 1; i++) {
        level = avl_get_val(*level, args[i]);
        if (!level)
            break;
    }

    if (level)
        avl_delete(level, args[num_args - 1]);

    puts("OK");
}


void cmd_print(Tree *world, char **args, int num_args) {
    Tree *level = world;

    for(int i = 0; i < num_args; i++) {
        level = avl_get_val(*level, args[i]);
        if(!level)
            return;
    }

    avl_print(*level);
}


void cmd_check(Tree *world, char **args, int num_args) {
    // check if the last argument isn't '*'
    if (strcmp(args[num_args - 1], "*") == 0) {
        fputs("ERROR\n", stderr);
        return;
    }

    if (avl_check(*world, args, num_args))
        puts("YES");
    else
        puts("NO");
}