#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#include "Config.h"
#include "stack.h"

FILE* logs = fopen("logs.txt", "w");

int StackCtor(struct stack* stk, size_t capacity)
{
    if (stk == NULL)
        return STACKPTRERR;

    char* buffer = (char*) calloc(1, capacity * sizeof(elem_t) + 2 * sizeof(unsigned long long));

    if (buffer == NULL)
        return MEMERR;

    stk->dataguardl = (unsigned long long*) buffer;
    stk->dataguardl[0] = CANARY;
    stk->data = (elem_t*) (buffer + sizeof(unsigned long long));
    stk->dataguardr = (unsigned long long*) (buffer + sizeof(unsigned long long) + capacity * sizeof(elem_t));
    stk->dataguardr[0] = CANARY;

    stk->canaryleft = CANARY;
    stk->capacity = capacity;
    stk->size = 0;
    stk->canaryright = CANARY;

    FillWPoison(stk, 0, (int)stk->capacity);

    return NOERR;
}

int FillWPoison(struct stack* stk, int left, int right)
{
    if (stk == NULL)
        return STACKPTRERR;

    for (int i = left; i < right; i++)
    {
        printf("%d", i);
        stk->data[i] = GetPoison(stk->data[0]);
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
    stk->data[stk->size] = GetPoison(stk->data[0]);

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

    char* prev = (char*) stk->dataguardl;

    char* buffer = (char*) realloc(stk->dataguardl, (stk->capacity) * sizeof(elem_t) + 2 * sizeof(unsigned long long));

    if (buffer == NULL)
        return MEMERR;

    stk->dataguardl = (unsigned long long*) buffer;
    stk->dataguardl[0] = CANARY;
    stk->data = (elem_t*)(buffer + sizeof(unsigned long long));
    stk->dataguardr = (unsigned long long*) (buffer + sizeof(unsigned long long) + stk->capacity * sizeof(elem_t));
    stk->dataguardr[0] = CANARY;

    if (prev != (char*) stk->dataguardl)
        free(prev);

    return NOERR;
}

int StackShrink(struct stack* stk)
{
    if (stk == NULL)
        return STACKPTRERR;

    char* buffer = (char*) realloc(stk->dataguardl, (stk->capacity) * sizeof(elem_t) + 2 * sizeof(unsigned long long));

    if (buffer == NULL)
        return MEMERR;

    stk->dataguardl = (unsigned long long*) buffer;
    stk->dataguardl[0] = CANARY;
    stk->data = (elem_t*)(buffer + sizeof(unsigned long long));
    stk->dataguardr = (unsigned long long*) (buffer + sizeof(unsigned long long) + stk->capacity * sizeof(elem_t));
    stk->dataguardr[0] = CANARY;

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
    if (stk.canaryleft != CANARY || stk.canaryright != CANARY || stk.dataguardl[0] != CANARY || stk.dataguardr[0] != CANARY)
        errors |= CANERR;

    return errors;
}

int StackDump(struct stack* stk, int errors, int line, const char* func, const char* file)
{
    FILE* logs = fopen("logs.txt", "a");

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
            if (stk->data[i] != GetPoison(stk->data[0]) && !isnan(stk->data[i]))
            {
                fprintf(logs, "\t\t*[%d] = ", i);
                print(logs, stk->data[i]);
            }
            else
                fprintf(logs, "\t\t[%d] = POISON\n", i);
        }
    }
    fprintf(logs, "\t}\n");
    fprintf(logs, "}\n");

    fclose(logs);

    return NOERR;
}

int StackDetor(struct stack* stk)
{
    if (stk == NULL)
    {
        return STACKPTRERR;
    }

    stk->canaryleft = 0;
    stk->canaryright = 0;
    stk->capacity = 0xDED32DED;
    free(stk->dataguardl);
    stk->data = NULL;
    stk->dataguardl = NULL;
    stk->dataguardr = NULL;
    stk->size = -1;

    return NOERR;
}

int print(FILE* fp, int x)
{
    return fprintf(fp, "%d\n", x);
}

int print(FILE* fp, double x)
{
    return fprintf(fp, "%lg\n", x);
}

int print(FILE* fp, char x)
{
    return fprintf(fp, "%c\n", x);
}

int print(FILE* fp, char* x)
{
    return fprintf(fp, "%p\n", x);
}

int print(FILE* fp, long x)
{
    return fprintf(fp, "%ld\n", x);
}

double GetPoison(double x)
{
    return NAN;
}

int GetPoison(int x)
{
    return 0xDED32DED;
}

char GetPoison(char x)
{
    return '\0';
}

char* GetPoison(char* x)
{
    return NULL;
}

long GetPoison(long x)
{
    return 0xDED32DED;
}


