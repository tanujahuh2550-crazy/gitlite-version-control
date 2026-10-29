
#include <stdio.h>
#include "stack.h"

int main(void)
{
    GitLiteStack stack;
    int value;

    if (!stack_init(&stack)) {
        printf("Stack initialization failed.\n");
        return 1;
    }

    printf("Stack initialized.\n");

    stack_push(&stack, 10);
    stack_push(&stack, 20);
    stack_push(&stack, 30);

    printf("Stack size: %zu\n", stack_size(&stack));

    if (stack_peek(&stack, &value))
        printf("Top element: %d\n", value);

    if (stack_pop(&stack, &value))
        printf("Popped element: %d\n", value);

    printf("Stack size after pop: %zu\n", stack_size(&stack));

    stack_clear(&stack);
    printf("Empty after clear: %s\n",
           stack_is_empty(&stack) ? "Yes" : "No");

    stack_destroy(&stack);
    printf("Stack memory released.\n");

    return 0;
}
