//
// Created by tux on 3/17/20.
//

#include "input.h"
#include "avl.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "commands.h"


int cmd_add(Tree *world, char **args, int n) {
    Tree *level = world;
    for (int i = 0; i < n; i++)
        if (avl_insert(level, args[i], &level) == -1)
            return -1;

    puts("OK");
    return 0;
}

void cmd_del(Tree *world, char **args, int n) {
    if (n == 0) {
        avl_free(world);
    }

    Tree *level = world;
    for (int i = 0; i < n - 1; i++) {
        level = avl_get_val(*level, args[i]);
        if (!level)
            break;
    }

    if (level)
        avl_delete(level, args[n - 1]);

    puts("OK");
}

void cmd_print(Tree *world, char **args, int n) {
    Tree *level = world;

    for(int i = 0; i < n; i++) {
        level = avl_get_val(*level, args[i]);
        if(!level)
            return;
    }

    avl_print(*level);
}

void cmd_check(Tree *world, char **args, int n) {
    if (strcmp(args[n - 1], "*") == 0) {
        fputs("ERROR\n", stderr);
        return;
    }

    if (avl_check(*world, args, n))
        puts("YES");
    else
        puts("NO");
}