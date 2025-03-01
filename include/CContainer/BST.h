#ifndef BST_H__
#define BST_H__

#include "CContainer/Type.h"

typedef struct BSTNode_s {
    Type data;
    BSTNode *_left;
    BSTNode *_right;
} BSTNode;

BSTNode *BSTNode_create(Type _data);
void BSTNode_destroy(BSTNode *_nbst, Fdtor _dtor);
void BSTNode_destroy_rec(BSTNode *_nbst, Fdtor _dtor);

typedef struct BSTRoot_s {
    size_t count;
    BSTNode *_root;
    Fcomp _comp;
    Fdtor _dtor;
} BSTRoot;

BSTRoot *BSTRoot_create(Fcomp _comp, Fdtor _dtor);
void BSTRoot_destroy(BSTRoot *_rbst);

void BSTRoot_add(BSTRoot *_rbst, Type _elem);
void BSTRoot_contain(BSTRoot *_rbst, Type _elem);

void BSTRoot_clear(BSTRoot *_rbst);
void BSTRoot_remove(BSTRoot *_rbst, Type _elem);

#endif