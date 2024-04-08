#ifndef UTILS_H__
#define UTILS_H__

#include <stddef.h>

#define max(_l, _r) (_l) > (_r) ? (_l) : (_r);
#define min(_l, _r) (_l) < (_r) ? (_l) : (_r);

void *alloc_zero(size_t _size);

#endif
