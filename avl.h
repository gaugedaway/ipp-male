#ifndef _AVL_H_
#define _AVL_H_


typedef struct avl {
    struct avl *c[2];
    const char *str;
    struct avl *val;
    int h;
} avl;


avl* avl_create(void);
void avl_insert(avl **node_ptr, const char *str);
void avl_delete(avl **node_ptr, const char *str);
int avl_check(const avl *node, const char *str);
avl** avl_get(avl *node, const char *str);
void avl_free(avl *node);
void avl_print(const avl *node, int indent);


#endif
