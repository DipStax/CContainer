
#include <stdlib.h>
#include <string.h>

#include "CContainer/Vector.h"
#include "CContainer/Utils.h"

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

void Vector_resize(Vector *_vec, size_t _size)
{
    if (_vec->_rsize < _size) {
        Vector_reserve(_vec, _size);
        _vec->size = _size;
        return;
    } else if (_vec->_rsize > _size) {
        for (size_t it = _vec->_rsize; it > _size; it--)
            _vec->_dtor(_vec->data + VEC_RSIZE(it, _vec));
        _vec->size = min(_size, _vec->size);
    }
}

void Vector_reserve(Vector *_vec, size_t _size)
{
    if (_vec->_rsize < _size) {
        size_t act_size = VEC_RSIZE(_vec->_rsize, _vec);
        size_t fut_size = VEC_RSIZE(_size, _vec);

        _vec->data = realloc(_vec->data, fut_size);
        memset(_vec->data + act_size, 0, fut_size - act_size);
        _vec->_rsize = _size;
    }
}

void Vector_fitreserve(Vector *_vec)
{
    _vec->data = realloc(_vec->data, VEC_RSIZE(_vec->size, _vec));
}

void Vector_append(Vector *_vec, Type _val)
{
    if (_vec->size == _vec->_rsize)
        Vector_resize(_vec, _vec->size + 1);
    memcpy(_vec->data + VEC_RSIZE(_vec->size, _vec), _val, _vec->_objsize);
    _vec->size++;
}

void Vector_prepend(Vector *_vec, Type _val)
{
    if (_vec->size == _vec->_rsize)
        Vector_resize(_vec, _vec->size + 1);
    priv_Vector_lshift(_vec, 0, _vec->size);
    memcpy(_vec->data, _val, _vec->_objsize);
    _vec->size++;
}

Type Vector_at(Vector *_vec, size_t _it)
{
    return _vec->data + _vec->_objsize * _it;
}

void Vector_clear(Vector *_vec)
{
    while (_vec->size)
        Vector_erase(_vec, 0);
}

void Vector_erase(Vector *_vec, size_t _it)
{
    size_t itpos = VEC_RSIZE(_it, _vec);
    _vec->_dtor(_vec->data + itpos);
    priv_Vector_lshift(_vec, _it, 1);
    _vec->size--;
}

void priv_Vector_lshift(Vector *_vec, size_t _offset, size_t _it)
{
    size_t offset = VEC_RSIZE(_offset, _vec);

    for (size_t it = 1; it <= _it; it++) {
        size_t itpos = VEC_RSIZE(it, _vec);

        memcpy(_vec->data + offset + (itpos - _vec->_objsize), _vec->data + offset + itpos, _vec->_objsize);
    }
}

void priv_Vector_rshift(Vector *_vec, size_t _offset, size_t _it)
{
    size_t offset = VEC_RSIZE(_offset, _vec);

    for (; _it > 0; _it--) {
        size_t itpos = VEC_RSIZE(_it, _vec);

        memcpy(_vec->data + offset + itpos, _vec->data + offset + (itpos - _vec->_objsize), _vec->_objsize);
    }
}