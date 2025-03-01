#include "CContainer/BST.h"
#include "CContainer/Utils.h"

BSTNode *BSTNode_create(Type _data)
{
    BSTNode *node = alloc_zero(sizeof(BSTNode));

    node->data = _data;
    return node;
}

void BSTNode_destroy(BSTNode *_nbst, Fdtor _dtor)
{
    _dtor(_nbst->data);
    free(_nbst);
}

void BSTNode_destroy_rec(BSTNode *_nbst, Fdtor _dtor)
{
    if (_nbst->_left)
        BSTNode_destroy_rec(_nbst->_left, _dtor);
    if (_nbst->_right)
        BSTNode_destroy_rec(_nbst->_right, _dtor);
    BSTNode_destroy(_nbst, _dtor);
}

BSTRoot *BSTRoot_create(Fcomp _comp, Fdtor _dtor)
{
    BSTRoot *root = alloc_zero(sizeof(BSTRoot));

    root->_comp = _comp;
    root->_dtor = _dtor;
    return root;
}

void BSTRoot_destroy(BSTRoot *_rbst)
{
    BSTRoot_clear(_rbst);
    free(_rbst);
}

void BSTRoot_add(BSTRoot *_rbst, Type _elem)
{
    if (_rbst->_root == NULL) {
        _rbst->_root = BSTNode_create(_elem);
    } else {
        priv_BSTRoot_add_rec(_rbst, _rbst->_root, _elem);
    }
}

void BSTRoot_add(BSTRoot *_rbst, Type _elem)
{
    BSTNode *parent = NULL;
    BSTNode *root = _rbst->_root;

    while (root) {
        int res = _rbst->_comp(_elem, root);
        parent = root;

        if (res > 0) {
            root = root->_left;
        } else if (res < 0) {
            root = root->_right;
        } else {
            raise(1);
        }
    }
    if (parent == NULL) {
        _rbst->_root = BSTNode_create(_elem);
    } else if (_rbst->_comp(_elem, root->data) > 0) {
        parent->_left = BSTNode_create(_elem);
    } else {
        parent->_right = BSTNode_create(_elem);
    }
}

void BSTRoot_clear(BSTRoot *_rbst)
{
    BSTNode_destroy_rec(_rbst->_root);
    _rbst->_root = NULL;
    _rbst->count = 0;
}

