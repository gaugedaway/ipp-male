#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "avl.h"


// Returns the height of the tree 'tree'.
static inline int height(const AVLTree tree) {
    if (!tree) return 0;
    return tree->height;
}


// Returns the maximum of values a and b.
static inline int max(int a, int b) {
    if (a > b) return a;
    return b;
}


// Sets the field ’height’ in the root of ’tree’
// to its proper value using the saved heights
// of its subtrees.
static inline void fix_height(AVLTree tree) {
    tree->height = max(height(tree->children[0]),
                       height(tree->children[1])) + 1;
}


// Performs a single rotation on a tree pointed to by 'tree'.
// ’child’ is 0 or 1 depending on whether we want the left or the right
// child of the root to become the new root after the rotation.
static inline void rot1(AVLTree *tree_ptr, int child) {
    AVLTree tree = *tree_ptr;
    AVLTree child_tree = tree->children[child];

    tree->children[child] = child_tree->children[!child];
    child_tree->children[!child] = tree;
    fix_height(tree);
    fix_height(child_tree);

    *tree_ptr = child_tree;
}


// Performs a double rotation on a tree pointed to by 'tree'.
// ’child’ is 0 or 1 depending on whether we want the
// right child of the left child or the left child of the right child
// of the root to become the new root after the rotation.
static inline void rot2(AVLTree *tree_ptr, int child) {
    rot1(&(*tree_ptr)->children[child], !child);
    rot1(tree_ptr, child);
}


// Balances the three using a suitable rotation on a tree pointed to by 'tree'.
static void balance(AVLTree *tree_ptr) {
    AVLTree tree = *tree_ptr;

    for (int child = 0; child <= 1; child++) {
        if (height(tree->children[child]) >
            height(tree->children[!child]) + 1) {
            AVLTree child_tree = tree->children[child];

            if (height(child_tree->children[child]) >
                height(child_tree->children[!child]))
                rot1(tree_ptr, child);
            else
                rot2(tree_ptr, child);

            return;
        }
    }
}


// Creates a new node with a given key and returns a pointer
// to it (a one-element tree).
static inline AVLTree create_node(const char *key) {
    AVLTree res = malloc(sizeof(Node));

    res->children[0] = res->children[1] = NULL;
    res->height = 1;

    res->key = malloc(strlen(key) + 1);
    strcpy(res->key, key);

    res->val = avl_create();

    return res;
}


// Searches for a node with key == 'key' in the tree
// pointed to by tree. If such a node doesn't exist
// it creates it and inserts it to the tree.
// Returns the pointer to a tree being a value corresponding
// to the key 'key' in the resulting tree. (For example if
// you 'key' is a name of a forest end 'tree' points to the
// tree representing the world, the return value is a pointer
// to a tree representing that forest).
AVLTree* avl_insert(AVLTree *tree_ptr, const char *key) {
    AVLTree tree = *tree_ptr;
    AVLTree *res = NULL;

    if (!tree) {
        *tree_ptr = create_node(key);
        return &(*tree_ptr)->val;
    }

    int v = strcmp(key, tree->key);
    if (v == 0) res = &tree->val;
    else if (v < 0) res = avl_insert(&tree->children[0], key);
    else res = avl_insert(&tree->children[1], key);

    fix_height(tree);
    balance(tree_ptr);

    return res;
}


// Destroys the tree pointed to by 'tree_ptr'.
void avl_free(AVLTree *tree_ptr) {
    AVLTree tree = *tree_ptr;
    if (!tree) return;
    avl_free(&tree->children[0]);
    avl_free(&tree->children[1]);
    avl_free(&tree->val);
    free(tree->key);
    free(tree);
    *tree_ptr = NULL;
}


// A helper function used in 'avl_delete'. Removes a node
// containing the smallest key inside from the tree pointed to by
// 'tree_ptr' and returns it (as a one-element tree).
// Assumes that the tree isn't empty.
static AVLTree extract_min(AVLTree *tree_ptr) {
    AVLTree tree = *tree_ptr;

    if (!tree) return NULL;

    if (tree->children[0]) {
        AVLTree res = extract_min(&tree->children[0]);
        fix_height(tree);
        balance(tree_ptr);
        return res;
    }

    *tree_ptr = tree->children[1];
    tree->children[1] = NULL;
    tree->height = 1;
    return tree;
}


// Delete the node corresponding to the key 'key' from the tree
// (if it doesn't exist, do nothing).
void avl_delete(AVLTree *tree_ptr, const char *key) {
    AVLTree tree = *tree_ptr;

    if (!tree) return;

    int v = strcmp(key, tree->key);
    if (v == 0) {   // the node to be removed is the root
        if (!tree->children[1]) {
            // If there is no right subtree, just replace the
            // root with it's left child.
            *tree_ptr = tree->children[0];
        }
        else {
            // Otherwise replace it with the smallest element
            // from the right subtree.
            *tree_ptr = extract_min(&tree->children[1]);
        }

        avl_free(&tree->val);
        free(tree->key);
        free(tree);
    }
    else if (v < 0) {
        avl_delete(&tree->children[0], key);
    }
    else {
        avl_delete(&tree->children[1], key);
    }

    fix_height(*tree_ptr);
    balance(tree_ptr);
}


// Returns 1 if key 'key' is in the tree 'tree', 0 otherwise.
int avl_check(const AVLTree tree, const char *key) {
    if (!tree) return 0;
    int v = strcmp(key, tree->key);
    if (v == 0) return 1;
    if (v < 0) return avl_check(tree->children[0], key);
    return avl_check(tree->children[1], key);
}


// Returns the pointer to the tree being the value corresponding
// to the key 'key' (so just like avl_insert, but when the key
// isn't found it returns NULL instead of inserting it).
AVLTree* avl_get_val(AVLTree tree, const char *key) {
    if (!tree) return NULL;
    int v = strcmp(key, tree->key);
    if (v == 0) return &tree->val;
    if (v < 0) return avl_get_val(tree->children[0], key);
    return avl_get_val(tree->children[1], key);
}


void avl_print(const AVLTree tree, int s) {
    if (!tree) return;
    avl_print(tree->children[1], s + 1);
    for (int i = 0; i < s; i++) putchar(' ');
    puts(tree->key);
    avl_print(tree->children[0], s + 1);
}


// Returns an empty tree.
AVLTree avl_create(void) {
    return NULL;
}
