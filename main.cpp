#include <stdio.h>

#include "stack.h"

int main()
{
    struct stack stk = {};

    StackCtor(&stk, 5);

    for (int i = 0; i < 25; i++)
        StackPush(&stk, i);

    for (int i = 25; i > 0; i--)
        printf("%d\n", StackPop(&stk));

    StackDetor(&stk);

    return 0;
}
