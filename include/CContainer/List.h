#ifndef LIST_H__
#define LIST_H__

#include "CContainer/IteratorList.h"

typedef struct List_s
{
    size_t size;
    IteratorList *head;
    Fdtor _dtor;
} List;

List *List_create(Fdtor _dtor);
void List_destroy(List *_list);

void List_append(List *_list, Type _val);
void List_prepend(List *_list, Type _val);

Type List_at(List *_list, size_t _it);

void List_erase(List *_list, size_t _it);

#endif