//
// Created by tux on 3/17/20.
//

#ifndef MALE_COMMANDS_H
#define MALE_COMMANDS_H

#include "avl.h"


void cmd_add(AVLTree *world, char **args, int n);

void cmd_del(AVLTree *world, char **args, int n);

void cmd_print(AVLTree *world, char **args, int n);

void cmd_check(AVLTree *world, char **args, int n);


#endif //MALE_COMMANDS_H
