#include <stdlib.h>

#include "CContainer/List.h"
#include "CContainer/Utils.h"

List *List_create(Fdtor _dtor)
{
    List *list = alloc_zero(sizeof(List));

    list->_dtor = _dtor;
    return list;
}

void List_destroy(List *_list)
{
    for (; _list->size != 0; _list->size--)
        List_erase(_list, 0);
}

void List_append(List *_list, Type _val)
{
    IteratorList *node = IteratorList_create(_val);
    IteratorList *last = NULL;

    if (_list->head) {
        last = priv_IteratorList_last(_list->head);
        priv_IteratorList_connect(last, node);
    } else {
        _list->head = node;
    }
    _list->size++;
}

void List_prepend(List *_list, Type _val)
{
    IteratorList *node = IteratorList_create(_val);

    priv_IteratorList_connect(node, _list->head);
    _list->head = node;
    _list->size++;
}

Type List_at(List *_list, size_t _it)
{
    return priv_IteratorList_at(_list->head, _it)->data;
}

void List_erase(List *_list, size_t _it)
{
    IteratorList *node = priv_IteratorList_at(_list->head, _it);

    if (node == _list->head)
        _list->head = _list->head->next;
    priv_IteratorList_connect(node->prev, node->next);
    IteratorList_destroy(node, _list->_dtor);
    _list->size--;
}