#include "CContainer/Vector.h"

Vector *Vector_create(size_t _objsize, Fdtor _dtor)
{
    Vector *vec = alloc_zero(sizeof(Vector));

    vec->_objsize = _objsize;
    vec->_dtor = _dtor;
    return vec;
}

void Vector_destroy(Vector *_vec)
{
    Vector_clear(_vec);
    free(_vec);
}

void Vector_reserve(Vector *_vec, size_t _size)
{
    if (_vec->_rsize < _size) {
        size_t act_size = _vec->_rsize * _vec->_objsize;
        size_t fut_size = _size * _vec->_objsize;

        realloc(_vec->data, fut_size);
        memset(_vec->data + act_size, 0, fut_size - act_size);
        _vec->_rsize = _size;
    }
}