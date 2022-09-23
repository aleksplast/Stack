#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "stack.h"

FILE* logs = fopen("logs.txt", "a");

int StackCtor(struct stack* stk, size_t capacity)
{
    if (stk == NULL)
    {
        return STACKPTRERR;
    }

    stk->capacity = capacity;
    stk->data = (elem_t*) calloc(capacity, sizeof(elem_t));
    stk->size = 0;

    FillWPoison(stk);

    return NOERR;
}

void FillWPoison(struct stack* stk)
{
    for (int i = 0; i < stk->capacity; i++)
    {
        stk->data[i] = NAN;
    }
}

elem_t StackPop(struct stack* stk)
{
    if (stk == NULL)
    {
        return STACKPTRERR;
    }

    if (int errors = StackErr(*stk))
        StackDump(stk, errors, __LINE__, __func__, __FILE__);

    elem_t value = stk->data[stk->size];
    stk->data[stk->size--] = NAN;

    return value;
}

int StackPush(struct stack* stk, elem_t elem)
{
    if (stk == NULL)
    {
        return STACKPTRERR;
    }

    if (int errors = StackErr(*stk))
        StackDump(stk, errors, __LINE__, __func__, __FILE__);

    if (stk->size >= stk->capacity)
        StackRealloc(stk);

    stk->data[stk->size++] = elem;

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
            if (!isnan(*(stk->data + i)))
                fprintf(logs, "\t\t*[%d] = %lg\n", i, *((stk->data) + i));
            else
                fprintf(logs, "\t\t[%d] = NAN (POISON)\n", i);
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
        fprintf(logs, "Stack pointer = NULL");
        return STACKPTRERR;
    }

    stk->capacity = NAN;
    stk->data = NULL;
    stk->size = NAN;

    return NOERR;
}
