/*
 * Author: Adam Bac
 * File: avl.c
 *
 * Implements a dictionary with strings as keys and other
 * dictionaries as values using AVL tree.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "avl.h"


/*
 * Returns an empty tree.
 */
Tree avl_create(void) {
    return NULL;
}


/*
 * Returns the height of the tree [tree].
 */
static inline int height(const Tree tree) {
    if (!tree)
        return 0;
    return tree->height;
}


/*
 * Returns the maximum of values [a] and [b].
 */
static inline int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}


/*
 * Sets the field [height] in the root of [tree] to its proper
 * value using the saved heights of its subtrees.
 */
static inline void fix_height(Tree tree) {
    tree->height = max(height(tree->children[0]),
                       height(tree->children[1])) + 1;
}


/*
 * Performs a single rotation on a tree pointed to by [tree_ptr].
 * [child] is 0 or 1 depending on whether we want the left or the right
 * child of the root to become the new root after the rotation.
 */
static inline void rot1(Tree *tree_ptr, int child) {
    Tree tree = *tree_ptr;
    Tree child_tree = tree->children[child];

    tree->children[child] = child_tree->children[!child];
    child_tree->children[!child] = tree;
    fix_height(tree);
    fix_height(child_tree);
    *tree_ptr = child_tree;
}


/*
 * Performs a double rotation on a tree pointed to by [tree_ptr].
 * [child] is 0 or 1 depending on whether we want the
 * right child of the left child or the left child of the right child
 * of the root to become the new root after the rotation.
 */
static inline void rot2(Tree *tree_ptr, int child) {
    rot1(&(*tree_ptr)->children[child], !child);
    rot1(tree_ptr, child);
}


/*
 * Balances the three using a suitable rotation
 * on a tree pointed to by [tree_ptrs].
 */
static void balance(Tree *tree_ptr) {
    Tree tree = *tree_ptr;
    if (!tree)
        return;

    for (int child = 0; child <= 1; child++) {
        if (height(tree->children[child]) >
            height(tree->children[!child]) + 1) {
            Tree child_tree = tree->children[child];

            if (height(child_tree->children[child]) >
                height(child_tree->children[!child]))
                rot1(tree_ptr, child);
            else
                rot2(tree_ptr, child);

            return;
        }
    }
}


/*
 * Creates a new node with a key [key] and returns a pointer
 * to it (a one-element tree).
 */
static inline Tree create_node(const char *key) {
    Tree result = malloc(sizeof(Node));

    result->children[0] = result->children[1] = NULL;
    result->height = 1;

    result->key = malloc(strlen(key) + 1);
    strcpy(result->key, key);

    result->val = avl_create();

    return result;
}


/*
 * Searches for a node with key [key] in the tree
 * pointed to by [tree_ptr]. If such a node doesn't exist
 * it creates it and inserts it into the tree.
 * Returns the pointer to a tree being a value corresponding
 * to the key [key] in the resulting tree. (For example if
 * your [key] is a name of a forest and [tree] points to the
 * tree representing the world, the return value is a pointer
 * to a tree representing that forest).
 */
Tree *avl_insert(Tree *tree_ptr, const char *key) {
    Tree tree = *tree_ptr;
    Tree *result = NULL;

    if (!tree) {
        *tree_ptr = create_node(key);
        return &(*tree_ptr)->val;
    }

    int compare_result = strcmp(key, tree->key);
    if (compare_result == 0)
        result = &tree->val;
    else if (compare_result < 0)
        result = avl_insert(&tree->children[0], key);
    else
        result = avl_insert(&tree->children[1], key);

    fix_height(tree);
    balance(tree_ptr);

    return result;
}


/*
 * Destroys the tree pointed to by [tree_ptr].
 */
void avl_free(Tree *tree_ptr) {
    Tree tree = *tree_ptr;
    if (!tree) return;

    avl_free(&tree->children[0]);
    avl_free(&tree->children[1]);
    avl_free(&tree->val);
    free(tree->key);
    free(tree);
    *tree_ptr = NULL;
}


/* A helper function used in [avl_delete]. Removes a node
 * containing the smallest key inside from the tree pointed to by
 * [tree_ptr] and returns it (as a one-element tree).
 * Assumes that the tree isn't empty.
 */
static Tree extract_min(Tree *tree_ptr) {
    Tree tree = *tree_ptr;
    if (!tree)
        return NULL;

    if (tree->children[0]) {
        Tree result = extract_min(&tree->children[0]);
        fix_height(tree);
        balance(tree_ptr);
        return result;
    }

    *tree_ptr = tree->children[1];
    tree->children[1] = NULL;
    tree->height = 1;
    return tree;
}


/* Delete the node corresponding to the key [key] from the tree
 * pointed to by [tree_ptr] (if it doesn't exist, do nothing).
 */
void avl_delete(Tree *tree_ptr, const char *key) {
    Tree tree = *tree_ptr;
    if (!tree)
        return;

    int compare_result = strcmp(key, tree->key);
    // The node to be removed is the root
    if (compare_result == 0) {
        if (!tree->children[1]) {
            // If there is no right subtree, just replace the
            // root with it's left child.
            *tree_ptr = tree->children[0];
        }
        else {
            // Otherwise replace it with the smallest element
            // from the right subtree.
            Tree extracted = extract_min(&tree->children[1]);
            extracted->children[0] = tree->children[0];
            extracted->children[1] = tree->children[1];
            *tree_ptr = extracted;
        }

        avl_free(&tree->val);
        free(tree->key);
        free(tree);
    }
    else if (compare_result < 0) {
        avl_delete(&tree->children[0], key);
    }
    else {
        avl_delete(&tree->children[1], key);
    }

    if (*tree_ptr) {
        fix_height(*tree_ptr);
        balance(tree_ptr);
    }
}


/*
 * If [num_keys] = 1, the function simply checks if a node
 * with a key keys[0] is in the tree [tree].
 * If [num_keys] > 1, it searches recursively, that is first it
 * finds a node with a keys[0] in the tree [tree], takes the tree
 * that the node has as its value and, searches for the next key
 * in this new tree, and repeats until there's one key left.
 * Then it checks the last tree for the last key and returns the
 * appropriate result. Any of the keys except keys[num_keys - 1]
 * (the last one) can be a wildcard - '*' - in which case the procedure
 * searches for next keys in the trees belonging to all nodes of the tree
 * at the level where '*' occurred.
 */
int avl_check(const Tree tree, char **keys, int num_keys) {
    if (num_keys == 0)
        return 1;

    if (!tree)
        return 0;

    // check if the key we currently look for is a literal or a wildcard '*'
    if (strcmp(keys[0], "*") != 0) {
        int compare_result = strcmp(keys[0], tree->key);

        if (compare_result == 0)
            return avl_check(tree->val, keys + 1, num_keys - 1);
        if (compare_result < 0)
            return avl_check(tree->children[0], keys, num_keys);
        return avl_check(tree->children[1], keys, num_keys);
    }
    else {
        return avl_check(tree->val, keys + 1, num_keys - 1) ||
               avl_check(tree->children[0], keys, num_keys) ||
               avl_check(tree->children[1], keys, num_keys);
    }
}


/*
 * Returns the pointer to the tree being the value corresponding
 * to the key [key] (so just like avl_insert, but when the key
 * isn't found it returns NULL instead of inserting it).
 */
Tree *avl_get_val(Tree tree, const char *key) {
    if (!tree)
        return NULL;

    int compare_result = strcmp(key, tree->key);
    if (compare_result == 0)
        return &tree->val;
    if (compare_result < 0)
        return avl_get_val(tree->children[0], key);
    return avl_get_val(tree->children[1], key);
}


/*
 * Prints all keys in the tree [tree] in lexicographic order.
 */
void avl_print(const Tree tree) {
    if (!tree)
        return;

    avl_print(tree->children[0]);
    puts(tree->key);
    avl_print(tree->children[1]);
}

