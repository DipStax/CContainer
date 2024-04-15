#ifndef VECTOR_H__
#define VECTOR_H__

#include <stddef.h>

#include "CContainer/Type.h"

#define MEM_VEC_RSIZE(_a, _v) (((_a) * (_v)->_objsize) / sizeof(void *))
#define ALC_VEC_RSIZE(_a, _v) ((_a) * (_v)->_objsize)

#define VEC_AT(_v, _t, _i) ((_t *)Vector_at(_v, _i))
#define VEC_AT_VAL(_v, _t, _i) (*VEC_AT(_v, _t, _i))

typedef struct Vector_s
{
    void *data;
    size_t size;
    size_t _objsize;
    size_t _rsize;
    Fdtor _dtor;
    void *_swap;
} Vector;

Vector *Vector_create(size_t _objsize, Fdtor _dtor);
void Vector_destroy(Vector *_vec);

void Vector_resize(Vector *_vec, size_t size);
void Vector_reserve(Vector *_vec, size_t _size);

void Vector_append(Vector *_vec, Type _val);
void Vector_prepend(Vector *_vec, Type _val);
void Vector_swap(Vector *_vec, size_t _i1, size_t _i2);

Type Vector_at(Vector *_vec, size_t _it);

void Vector_clear(Vector *_vec);
void Vector_erase(Vector *_vec, size_t _it);


void priv_Vector_lshift(Vector *_vec, size_t _offset, size_t _it);
void priv_Vector_rshift(Vector *_vec, size_t _offset, size_t _it);

#endif