#ifndef TYPE_H__
#define TYPE_H__

typedef void *Type;

typedef void (*Fdtor)(Type _arg);
typedef int (*Fcomp)(Type _left, Type _right);

#endif