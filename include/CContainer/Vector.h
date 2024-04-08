#ifndef VECTOR_H__
#define VECTOR_H__

#include <stddef.h>

#include "CContainer/Type.h"

typedef struct Vector_s
{
    void *data;
    size_t size;
    size_t _objsize;
    size_t _rsize;
    Fdtor _dtor;
} Vector;

Vector *Vector_create(size_t _objsize, Fdtor _dtor);
void Vector_destroy(Vector *_vec);

void Vector_resize(Vector *_vec, size_t size);
void Vector_reserve(Vector *_vec, size_t _size);

void Vector_append(Vector *_vec, Type _val);
void Vector_prepend(Vector *_vec, Type _val);

Type Vector_at(Vector *_vec, size_t _it);

void Vector_clear(Vector *_vec);
void Vector_erase(Vector *_vec, size_t _it);

#endif