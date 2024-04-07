#include <stdlib.h>
#include <string.h>

#include "CContainer/Utils.h"

void *alloc_zero(size_t _size)
{
    void *ptr = malloc(_size);

    memset(ptr, 0, _size);
    return ptr;
}