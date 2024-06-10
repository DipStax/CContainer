#include "CContainer/Vector.h"
#include <stdio.h>
#include <stdlib.h>

static void no_destroy(Type _value)
{
}

int main()
{
    size_t literal_value1 = 128;
    size_t literal_value2 = 256;
    Vector *vec;

    Type value1;
    Type value2;

    vec = Vector_create(sizeof(size_t), &no_destroy);

    value1 = malloc(sizeof(size_t));
    (*(size_t *)(value1)) = literal_value1;

    value2 = malloc(sizeof(size_t));
    (*(size_t *)(value2)) = literal_value2;
    Vector_append(vec, value1);
    Vector_append(vec, value2);
    printf("%zu == %zu\n", VEC_AT_VAL(vec, size_t, 0), literal_value1);
    printf("%zu == %zu\n", VEC_AT_VAL(vec, size_t, 1), literal_value2);
}