/*
 * Author: Adam Bac
 * File: avl.h
 *
 * Implements a dictionary with strings as keys and other
 * dictionaries as values using AVL tree.
 */


#ifndef _AVL_H_
#define _AVL_H_


typedef struct Node *Tree;

typedef struct Node {
    Tree children[2];
    Tree val;
    int height;
    char *key;
} Node;


Tree avl_create(void);

Tree *avl_insert(Tree *tree_ptr, const char *key);

void avl_delete(Tree *tree_ptr, const char *key);

int avl_check(const Tree tree, char **keys, int num_keys);

Tree *avl_get_val(Tree tree, const char *key);

void avl_free(Tree *tree_ptr);

void avl_print(const Tree tree);


#endif
