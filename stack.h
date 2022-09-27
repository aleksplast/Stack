#ifndef STACK_H
#define STACK_H

#include "Config.h"

struct stack{
    unsigned long long canaryleft;
    unsigned long long* dataguardl;
    elem_t* data;
    unsigned long long* dataguardr;
    size_t size;
    size_t capacity;
    unsigned long long canaryright;
};

enum StackErrors{
    NOERR = 0,
    STACKPTRERR = 1,
    DATAERR = 2,
    SIZERR = 4,
    CAPERR = 8,
    SIZENCAPERR = 16,
    CANERR = 32
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

int print(FILE* fp, long x);

int print(FILE* fp, char* x);

int print(FILE* fp, char x);

int print(FILE* fp, double x);

int print(FILE* fp, int x);

double GetPoison(double x);

int GetPoison(int x);

char GetPoison(char x);

long GetPoison(long x);

char* GetPoison(char* x);

#endif //STACK_H
