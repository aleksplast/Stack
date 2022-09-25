#ifndef STACK_H
#define STACK_H

#include "Config.h"

struct stack{
    elem_t* data;
    size_t size;
    size_t capacity;
};

enum StackErrors{
    NOERR = 0,
    STACKPTRERR = 1,
    DATAERR = 2,
    SIZERR = 4,
    CAPERR = 8,
    SIZENCAPERR = 16
};

enum SysErrors{
    MEMERR = -1
};

int StackCtor(struct stack* stk, size_t capacity);

int FillWPoison(struct stack* stk, int left, int right);

elem_t StackPop(struct stack* stk);

int StackPush(struct stack* stk, elem_t elem);

int StackRealloc(struct stack* stk);

int StackShrink(struct stack* stk);

int StackErr(struct stack stk);

int StackDump(struct stack* stk, int errors, int line, const char* func, const char* file);

int StackDetor(struct stack* stk);

int print(long x);

int print(char* x);

int print(char x);

int print(double x);

int print(int x);

#endif //STACK_H
