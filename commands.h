//
// Created by tux on 3/17/20.
//

#ifndef MALE_COMMANDS_H
#define MALE_COMMANDS_H

#include "avl.h"


int cmd_add(Tree *world, char **args, int n);

void cmd_del(Tree *world, char **args, int n);

void cmd_print(Tree *world, char **args, int n);

void cmd_check(Tree *world, char **args, int n);


#endif //MALE_COMMANDS_H
