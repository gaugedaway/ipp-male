#ifndef _AVL_H_
#define _AVL_H_


typedef struct Node* AVLTree;

typedef struct Node {
    AVLTree children[2];
    AVLTree val;
    int height;
    char *key;
} Node;


AVLTree avl_create(void);
AVLTree* avl_insert(AVLTree *tree_ptr, const char *key);
void avl_delete(AVLTree *tree_ptr, const char *key);
int avl_check(const AVLTree tree, const char *key);
AVLTree* avl_get_val(AVLTree tree, const char *key);
void avl_free(AVLTree *tree_ptr);
void avl_print(const AVLTree tree, int indent);


#endif
