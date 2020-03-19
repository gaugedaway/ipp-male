//
// Created by tux on 3/17/20.
//

#include "readline.h"
#include "avl.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "commands.h"

void cmd_add(AVLTree *world, char **args, int n) {
    AVLTree *level = world;
    for (int i = 0; i < n; i++)
        level = avl_insert(level, args[i]);
    puts("OK");
}

void cmd_del(AVLTree *world, char **args, int n) {
    if (n == 0) {
        avl_free(world);
    }

    AVLTree *level = world;
    for (int i = 0; i < n - 1; i++) {
        level = avl_get_val(*level, args[i]);
        if (!level)
            break;
    }

    if (level)
        avl_delete(level, args[n - 1]);

    puts("OK");
}

void cmd_print(AVLTree *world, char **args, int n) {
    AVLTree *level = world;

    for(int i = 0; i < n; i++) {
        level = avl_get_val(*level, args[i]);
        if(!level)
            return;
    }

    avl_print(*level);
}

void cmd_check(AVLTree *world, char **args, int n) {
    if (strcmp(args[n - 1], "*") == 0) {
        fputs("ERROR\n", stderr);
        return;
    }

    if (avl_check(*world, args, n))
        puts("YES");
    else
        puts("NO");
}