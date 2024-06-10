#include <stdio.h>
#include <stdarg.h>

#include "CContainer/Vector.h"
#include "Helper.h"

void hLog(char *_pfix, char *_user, ...)
{
    va_list args;
    va_start(args, _user);

    fprintf(stderr, "%s ", _pfix);
    vfprintf(stderr, _user, args);
    fprintf(stderr, "\n");
}

#define VECTOR_LOG(_s, ...) hLog("test-[Vector]", _s, __VA_ARGS__)

static void test_with_simple()
{
    hSimple_t *simple = random_simple();

    Vector *vec = Vector_create(sizeof(hSimple_t), &dtor_simple);

    Vector_append(vec, simple);
    VECTOR_LOG("Append 1 - New size: %zu,\trsize: %zu", vec->size, vec->_rsize);
    Vector_append(vec, random_simple());
    VECTOR_LOG("Append 2 - New size: %zu,\trsize: %zu", vec->size, vec->_rsize);

    Vector_erase(vec, 0);
    VECTOR_LOG("Erase - New size: %zu,\trsize: %zu", vec->size, vec->_rsize);

    Vector_reserve(vec, 5);
    VECTOR_LOG("Reserve - rsize: %zu", vec->_rsize);

    Vector_append(vec, random_simple());
    VECTOR_LOG("Append 3 - New size: %zu,\trsize: %zu", vec->size, vec->_rsize);

    Vector_prepend(vec, random_simple());
    VECTOR_LOG("Prepend - New size: %zu,\trsize: %zu", vec->size, vec->_rsize);

    Vector_clear(vec);
    VECTOR_LOG("Clear - New size: %zu,\trsize: %zu", vec->size, vec->_rsize);

    Vector_fitreserve(vec);
    VECTOR_LOG("FitReserve - New size: %zu,\trsize: %zu", vec->size, vec->_rsize);

    Vector_destroy(vec);
    // the variable "simple" is freed by the function "Vector_clear"
}

void vector_function()
{
    test_with_simple();
}