#ifndef ITERATORLIST_H__
#define ITERATORLIST_H__

#include <stddef.h>

typedef void *Type;

typedef struct IteratorList_s IteratorList;
typedef void (*Fdtor)(Type _arg);

struct IteratorList_s
{
    Type data;
    IteratorList *next;
    IteratorList *prev;
};


IteratorList *IteratorList_create(Type _val);
void IteratorList_destroy(IteratorList *_node, Fdtor _dtor);

IteratorList *priv_IteratorList_last(IteratorList *_node);
void priv_IteratorList_connect(IteratorList *_first, IteratorList *_second);
IteratorList *priv_IteratorList_at(IteratorList *_node, size_t _it);

#endif