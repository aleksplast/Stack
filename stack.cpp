#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#include "Config.h"
#include "stack.h"

FILE* logs = fopen("logs.txt", "w");
const int POISON = 666;

int StackCtor(struct stack* stk, size_t capacity)
{
    if (stk == NULL)
        return STACKPTRERR;

    stk->capacity = capacity;
    stk->data = (elem_t*) calloc(capacity, sizeof(elem_t));
    stk->size = 0;

    FillWPoison(stk, 0, stk->capacity);

    return NOERR;
}

int FillWPoison(struct stack* stk, int left, int right)
{
    if (stk == NULL)
        return STACKPTRERR;

    for (int i = left; i < right; i++)
    {
        stk->data[i] = POISON;
    }

    return NOERR;
}

elem_t StackPop(struct stack* stk)
{
    if (stk == NULL)
        return STACKPTRERR;

    int errors = StackErr(*stk);

    stk->size--;
    elem_t value = stk->data[stk->size];
    stk->data[stk->size] = POISON;

    if (stk->size < stk->capacity/2 - stk->capacity/8)
    {
        stk->capacity /= 2;
        StackShrink(stk);
    }

    StackDump(stk, errors, __LINE__, __func__, __FILE__);

    return value;
}

int StackPush(struct stack* stk, elem_t elem)
{
    if (stk == NULL)
        return STACKPTRERR;

    int errors = StackErr(*stk);
    StackDump(stk, errors, __LINE__, __func__, __FILE__);

    if (stk->size >= stk->capacity)
    {
        stk->capacity *= 2;
        StackRealloc(stk);
        FillWPoison(stk, stk->size + 1, stk->capacity);
    }

    stk->data[stk->size++] = elem;

    errors = StackErr(*stk);
    StackDump(stk, errors, __LINE__, __func__, __FILE__);

    return NOERR;
}

int StackRealloc(struct stack* stk)
{
    if (stk == NULL)
        return STACKPTRERR;

    elem_t* prev = stk->data;

    if ((stk->data = (elem_t*) realloc(stk->data, stk->capacity * sizeof(elem_t))) == NULL)
        return MEMERR;

    if (prev != stk->data)
        free(prev);

    return NOERR;
}

int StackShrink(struct stack* stk)
{
    if (stk == NULL)
        return STACKPTRERR;

    if ((stk->data = (elem_t*) realloc(stk->data, stk->capacity * sizeof(elem_t))) == NULL)
        return MEMERR;

    return NOERR;
}

int StackErr(struct stack stk)
{
    int errors = 0;

    if (&stk == NULL)
        return STACKPTRERR;
    if (stk.data == NULL)
        errors |= DATAERR;
    if (stk.size < 0 || isnan(stk.size))
        errors |= SIZERR;
    if (stk.capacity < 0 || isnan(stk.capacity))
        errors |= CAPERR;
    if (stk.size > stk.capacity)
        errors |= SIZENCAPERR;

    return errors;
}

int StackDump(struct stack* stk, int errors, int line, const char* func, const char* file)
{
    if (stk == NULL)
    {
        fprintf(logs, "ERROR: NULL Pointer to a stack");
        return STACKPTRERR;
    }

    fprintf(logs, "\n%s at ", func);
    fprintf(logs, "%s", file);
    fprintf(logs, "(%d)\n", line);
    fprintf(logs, "Stack[%p] ", &stk);
    if (errors == 0)
        fprintf(logs, "(OK)\n");
    else
        fprintf(logs, "(ERROR CODE = %d)\n", errors);
    fprintf(logs, "{\n\tsize = %lu\n", stk->size);
    fprintf(logs, "\tcapacity = %lu\n", stk->capacity);
    fprintf(logs, "\tdata[%p]\n\t{\n", stk->data);
    if (stk->data != NULL)
    {
        for (int i = 0; i < stk->capacity; i++)
        {
            if (stk->data[i] != POISON)
            {
                fprintf(logs, "\t\t*[%d] = ", i);
                print(stk->data[i]);
            }
            else
                fprintf(logs, "\t\t[%d] = POISON\n", i);
        }
    }
    fprintf(logs, "\t}\n");
    fprintf(logs, "}\n");

    return NOERR;
}

int StackDetor(struct stack* stk)
{
    if (stk == NULL)
    {
        return STACKPTRERR;
    }

    stk->capacity = 0xDED32DED;
    stk->data = NULL;
    stk->size = -1;

    return NOERR;
}

int print(int x)
{
    fprintf(logs, "%d\n", x);
}

int print(double x)
{
    fprintf(logs, "%lg\n", x);
}

int print(char x)
{
    fprintf(logs, "%c\n", x);
}

int print(char* x)
{
    fprintf(logs, "%p\n", x);
}

int print(long x)
{
    fprintf(logs, "%ld\n", x);
}
