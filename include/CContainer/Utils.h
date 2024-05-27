#ifndef UTILS_H__
#define UTILS_H__

#include <stddef.h>
#include <string.h>

#define max(_l, _r) ((_l) > (_r) ? (_l) : (_r))
#define min(_l, _r) ((_l) < (_r) ? (_l) : (_r))

#define ALLOC_HEAP(_t, _v) (memcpy(malloc(sizeof(_t)), &_v, sizeof(_t)))

void *alloc_zero(size_t _size);

#endif
