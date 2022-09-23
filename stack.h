#ifndef STACK_H
#define STACK_H

typedef double elem_t;

struct stack{
    elem_t* data;
    size_t size;
    size_t capacity;
};

enum Errors{
    NOERR = 0,
    STACKPTRERR = 1,
    DATAERR = 2,
    SIZERR = 4,
    CAPERR = 8,
    SIZENCAPERR = 16
};

int StackCtor(struct stack* stk, size_t capacity);

void FillWPoison(struct stack* stk);

elem_t StackPop(struct stack* stk);

int StackPush(struct stack* stk, elem_t elem);

int StackErr(struct stack stk);

int StackDump(struct stack* stk, int errors, int line, const char* func, const char* file);

int StackDetor(struct stack* stk);

#endif //STACK_H
