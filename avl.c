#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "avl.h"


static inline int height(const avl *t) {
    if(!t) return 0;
    return t->h;
}


static inline int max(int a, int b) {
    if(a > b) return a;
    return b;
}


static inline void fix_height(avl *t) {
    t->h = max(height(t->c[0]), height(t->c[1])) + 1;
}


static inline void rot1(avl **node_ptr, int child) {
    avl *node = *node_ptr;
    avl *child_node = node->c[child];

    node->c[child] = child_node->c[!child];
    child_node->c[!child] = node;
    fix_height(node);
    fix_height(child_node);

    *node_ptr = child_node;
}


static inline void rot2(avl **node_ptr, int child) {
    rot1(&(*node_ptr)->c[child], !child);
    rot1(node_ptr, child);
}


static void balance(avl **node_ptr) {
    avl *node = *node_ptr;
    for(int child = 0; child <= 1; child++) {
        if(height(node->c[child]) > height(node->c[!child]) + 1) {
            avl *child_node = node->c[child];
            if(height(child_node->c[child]) > height(child_node->c[!child])) {
                rot1(node_ptr, child);
            }
            else {
                rot2(node_ptr, child);
            }
            return;
        }
    }
}


static inline avl* create_node(const char *str) {
    avl *n = malloc(sizeof *n);
    n->c[0] = n->c[1] = NULL;
    n->h = 1;
    n->str = str;
    n->val = avl_create();
    return n;
}


void avl_insert(avl **node_ptr, const char *str) {
    avl *node = *node_ptr;
    if(!node) {
        *node_ptr = create_node(str);
        return;
    }
    int res = strcmp(str, node->str);
    if(res == 0) return;
    if(res < 0) avl_insert(&node->c[0], str);
    else avl_insert(&node->c[1], str);
    fix_height(node);
    balance(node_ptr);
}


static avl* extract_min(avl **node_ptr) {
    avl *node = *node_ptr;

    if(!node) return NULL;

    if(node->c[0]) {
        avl *res = extract_min(&node->c[0]);
        fix_height(node);
        balance(node_ptr);
        return res;
    }

    *node_ptr = node->c[1];
    return node;
}


void avl_free(avl *node) {
    if(!node) return;
    avl_free(node->c[0]);
    avl_free(node->c[1]);
    avl_free(node->val);
    free(node);
}


void avl_delete(avl **node_ptr, const char *str) {
    avl *node = *node_ptr;

    if(!node) return;

    int res = strcmp(str, node->str);
    if(res == 0) {
        *node_ptr = extract_min(&node->c[1]);
        avl_free(node->val);
        free(node);
    }
    else if(res < 0) avl_delete(&node->c[0], str);
    else avl_delete(&node->c[1], str);

    fix_height(*node_ptr);
    balance(node_ptr);
}


int avl_check(const avl *node, const char *str) {
    if(!node) return 0;
    int res = strcmp(str, node->str);
    if(res == 0) return 1;
    if(res < 0) return avl_check(node->c[0], str);
    return avl_check(node->c[1], str);
}


avl** avl_get(avl *node, const char *str) {
    if(!node) return NULL;
    int res = strcmp(str, node->str);
    if(res == 0) return &node->val;
    if(res < 0) return avl_get(node->c[0], str);
    return avl_get(node->c[1], str);
}


void avl_print(const avl *node, int s) {
    if(!node) return;
    avl_print(node->c[1], s + 1);
    for(int i=0; i<s; i++) putchar(' ');
    printf("%s\n", node->str);
    avl_print(node->c[0], s + 1);
}


avl* avl_create(void) {
    return NULL;
}
