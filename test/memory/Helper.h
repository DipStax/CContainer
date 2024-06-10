#ifndef TEST_MEMORY_HELPER__
#define TEST_MEMORY_HELPER__

#include <math.h>
#include <stdlib.h>

typedef struct hSimple_s {
    int intergral;
    float floating;
    char character;
} hSimple_t;

hSimple_t *random_simple()
{
    // srand(time(NULL));
    hSimple_t *res = (hSimple_t *)malloc(sizeof(hSimple_t));

    res->intergral = rand();
    if (res->intergral == 0)
        res->floating = M_PI;
    else
        res->floating = res->intergral * M_PI * 0.95;
    res->character = rand() % 8;
    return res;
}

void dtor_simple(void *_hsimple)
{
    hSimple_t *hsimple = (hSimple_t *)_hsimple;

    free(hsimple);
}

#endif