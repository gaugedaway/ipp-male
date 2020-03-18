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
    AVLTree *forest = avl_insert(world, args[0]);
    if (n >= 2) {
        AVLTree *tree = avl_insert(forest, args[1]);
        if (n == 3) {
            avl_insert(tree, args[2]);
        }
    }

    puts("OK");
}

void cmd_del(AVLTree *world, char **args, int n) {
    if (n == 0) {
        avl_free(world);
    } else if (n == 1) {
        avl_delete(world, args[0]);
    } else {
        AVLTree *forest = avl_get_val(*world, args[0]);
        if (forest) {
            if (n == 2) {
                avl_delete(forest, args[1]);
            }
            else {
                AVLTree *tree = avl_get_val(*forest, args[1]);
                if (tree)
                    avl_delete(tree, args[2]);
            }
        }
    }

    puts("OK");
}

void cmd_print(AVLTree *world, char **args, int n) {
    if (n == 0) {
        avl_print(*world);
    } else {
        AVLTree *forest = avl_get_val(*world, args[0]);
        if (!forest) return;
        if (n == 1) {
            avl_print(*forest);
        } else {
            AVLTree *tree = avl_get_val(*forest, args[1]);
            if (!tree) return;
            avl_print(*tree);
        }
    }
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