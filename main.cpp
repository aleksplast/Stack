#include <stdio.h>

#include "stack.h"

int main()
{
    FILE* logs = fopen("logs.txt", "w");

    struct stack stk;

    StackCtor(&stk, 5);

    for (int i = 0; i < 25; i++)
        StackPush(&stk, i);

    for (int i = 25; i > 0; i--)
        printf("%d\n", StackPop(&stk));

    StackDetor(&stk);

    fclose(logs);

    return 0;
}
