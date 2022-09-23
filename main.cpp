#include <stdio.h>

#include "stack.h"

int main()
{
    struct stack stk = {};
    size_t cap = 0;

    StackCtor(&stk, 5);

    stk.data = NULL;

    if (int errors = StackErr(stk))
        StackDump(&stk, errors, __LINE__, __func__, __FILE__);

    StackDetor(&stk);

    return 0;
}
