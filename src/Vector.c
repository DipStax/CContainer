
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>

#include "CContainer/Vector.h"
#include "CContainer/Utils.h"

Vector *Vector_create(size_t _objsize, Fdtor _dtor)
{
    Vector *vec = alloc_zero(sizeof(Vector));

    vec->_objsize = _objsize;
    vec->_dtor = _dtor;
    vec->_swap = malloc(_objsize);
    return vec;
}

void Vector_destroy(Vector *_vec)
{
    Vector_clear(_vec);
    free(_vec);
}

void Vector_resize(Vector *_vec, size_t _size)
{
    VEC_INFO("Resize - entry", _vec);
    if (_vec->_rsize < _size) {
        printf("[%s] Resize - 1 | required reserve\n", vec_str);
        Vector_reserve(_vec, _size);
        _vec->size = _size;
        VEC_INFO("Resize - 1 | new size", _vec);
        return;
    } else if (_vec->_rsize > _size) {
        printf("[%s] Resize - 2 | detruct %zu item\n", vec_str, _size - _vec->_rsize);
        for (size_t it = _vec->_rsize; it > _size; it--)
            _vec->_dtor(_vec->data + MEM_VEC_RSIZE(it, _vec));
        _vec->size = min(_size, _vec->size);
        VEC_INFO("Resize - 2 | new size", _vec);
    }
}

void Vector_reserve(Vector *_vec, size_t _size)
{
    if (_vec->_rsize < _size) {
        size_t act_size = MEM_VEC_RSIZE(_vec->_rsize, _vec);
        size_t fut_size = MEM_VEC_RSIZE(_size, _vec);

        _vec->data = realloc(_vec->data, fut_size);
        memset(_vec->data + act_size, 0, fut_size - act_size);
        _vec->_rsize = _size;
    }
}

void Vector_fitreserve(Vector *_vec)
{
    _vec->data = realloc(_vec->data, MEM_VEC_RSIZE(_vec->size, _vec));
}

void Vector_append(Vector *_vec, Type _val)
{
    _vec->size++;
    if (_vec->size >= _vec->_rsize)
        Vector_resize(_vec, _vec->size);
    memcpy(_vec->data + MEM_VEC_RSIZE(_vec->size - 1, _vec), _val, _vec->_objsize);
}

void Vector_prepend(Vector *_vec, Type _val)
{
    _vec->size++;
    if (_vec->size == _vec->_rsize)
        Vector_resize(_vec, _vec->size);
    priv_Vector_lshift(_vec, 0, _vec->size);
    memcpy(_vec->data, _val, _vec->_objsize);
}

void Vector_swap(Vector *_vec, size_t _i1, size_t _i2)
{
    char tmp[_vec->_objsize];

    memcpy(tmp, _vec->data + _i1, _vec->_objsize);
    memcpy(_vec->data + _i1, _vec->data + _i2, _vec->_objsize);
    memcpy(_vec->data + _i2, tmp, _vec->_objsize);
}

Type Vector_at(Vector *_vec, size_t _it)
{
    VEC_INFO("At | accessing", _vec);
    if (_it >= _vec->size) {
        printf("[%s] At - 1 | raise: index: %zu\n", vec_str, _it);
        VEC_INFO("At - 1 | raise:", _vec);
        raise(SIGSEGV);
    }
    printf("[%s] At - 2 | access item: %p, original address: %p\n", vec_str, _vec->data, _vec->data + _vec->_objsize * _it);
    return _vec->data + _vec->_objsize * _it;
}

void Vector_clear(Vector *_vec)
{
    VEC_INFO("Clear | start", _vec);
    while (_vec->size)
        Vector_erase(_vec, 0);
    VEC_INFO("Clear | clear done", _vec);
}

void Vector_erase(Vector *_vec, size_t _it)
{
    size_t itpos = MEM_VEC_RSIZE(_it, _vec);

    VEC_INFO("Erase | start", _vec);
    printf("[%s] Erase | index address offset: %zu, orignal: %zu\n", vec_str, itpos, _it);
    if (_it >= _vec->size) {
        printf("[%s] Erase -1 | raise: index: %zu\n", vec_str, _it);
        VEC_INFO("Erase - 1 | raise:", _vec);
        raise(SIGSEGV);
    }
    printf("[%s] Resize - 2 | detruct item: %p, original address: %p\n", vec_str, _vec->data, _vec->data + itpos);
    _vec->_dtor(_vec->data + itpos);
    priv_Vector_lshift(_vec, _it, 1);
    _vec->size--;
    VEC_INFO("Erase - 2 | new size", _vec);
}

void priv_Vector_lshift(Vector *_vec, size_t _offset, size_t _it)
{
    VEC_INFO("PRIV lshift | start", _vec);
    size_t offset = MEM_VEC_RSIZE(_offset, _vec);
    printf("[%s] PRIV lshift | offset: %zu, orignal offset: %zu\n", vec_str, offset, _offset);

    for (size_t it = 1; it <= _it; it++) {
        size_t itpos = MEM_VEC_RSIZE(it, _vec);
        printf("[%s] PRIV lshift | iteration: %zu, offset position: %zu\n", vec_str, it, itpos);

        printf("[%s] PRIV lshift | copy to: %p, copy from: %p, with size: %zu\n", vec_str, _vec->data + offset + MEM_VEC_RSIZE(it - 1, _vec), _vec->data + offset + itpos, _vec->_objsize);
        memcpy(_vec->data + offset + MEM_VEC_RSIZE(it - 1, _vec), _vec->data + offset + itpos, _vec->_objsize);
    }
}

void priv_Vector_rshift(Vector *_vec, size_t _offset, size_t _it)
{
    size_t offset = MEM_VEC_RSIZE(_offset, _vec);
    const size_t mem_objsize = _vec->_objsize / sizeof(void *);

    for (; _it > 0; _it--) {
        size_t itpos = MEM_VEC_RSIZE(_it, _vec);

        memcpy(_vec->data + offset + itpos, _vec->data + offset + (itpos - mem_objsize), mem_objsize);
    }
}