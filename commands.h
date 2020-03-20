/*
 * File: commands.h
 * Author: Adam Bac
 *
 * Implements executing four commands:
 * ADD, DEL, PRINT and CHECK.
 */

#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "avl.h"

int cmd_add(Tree *world, char **args, int num_args);

void cmd_del(Tree *world, char **args, int num_args);

void cmd_print(Tree *world, char **args, int num_args);

void cmd_check(Tree *world, char **args, int num_args);

#endif
