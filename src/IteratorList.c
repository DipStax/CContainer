#include <stdlib.h>
#include <signal.h>

#include "CContainer/IteratorList.h"
#include "CContainer/Utils.h"

IteratorList *priv_IteratorList_last(IteratorList *_node)
{
    if (_node->next)
        return priv_IteratorList_last(_node->next);
    return _node;
}

void priv_IteratorList_connect(IteratorList *_first, IteratorList *_second)
{
    if (_first)
        _first->next = _second;
    if (_second)
        _second->prev = _first;
}

IteratorList *priv_IteratorList_at(IteratorList *_node, size_t _it)
{
    if (_it != 0)
        return priv_IteratorList_at(_node->next, --_it);
    if (!_node)
        raise(SIGSEGV);
    return _node;
}

IteratorList *IteratorList_create(Type _val)
{
    IteratorList *node = alloc_zero(sizeof(IteratorList));

    node->data = _val;
    return node;
}

void IteratorList_destroy(IteratorList *_node, Fdtor _dtor)
{
    _dtor(_node->data);
    if (_node->prev)
        _node->prev->next = _node->next;
    if (_node->next)
        _node->next->prev = _node->prev;
    free(_node);
}