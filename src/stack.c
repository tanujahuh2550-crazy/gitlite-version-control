#include "stack.h"

#include <stdint.h>
#include <stdlib.h>

#define STACK_INITIAL_CAPACITY ((size_t)8)

int stack_init(GitLiteStack *stack)
{
    if (stack == NULL) {
        return 0;
    }

    stack->items = NULL;
    stack->size = 0;
    stack->capacity = 0;
    return 1;
}

int stack_push(GitLiteStack *stack, int value)
{
    int *new_items;
    size_t new_capacity;

    if (stack == NULL) {
        return 0;
    }

    if (stack->size == stack->capacity) {
        if (stack->capacity == 0) {
            new_capacity = STACK_INITIAL_CAPACITY;
        } else {
            if (stack->capacity > SIZE_MAX / 2) {
                return 0; /* Avoid overflow while growing the array. */
            }
            new_capacity = stack->capacity * 2;
        }

        if (new_capacity > SIZE_MAX / sizeof(*stack->items)) {
            return 0;
        }

        new_items = (int *)realloc(stack->items, new_capacity * sizeof(*stack->items));
        if (new_items == NULL) {
            return 0; /* Keep the old stack intact on allocation failure. */
        }

        stack->items = new_items;
        stack->capacity = new_capacity;
    }

    stack->items[stack->size] = value;
    stack->size++;
    return 1;
}

int stack_pop(GitLiteStack *stack, int *out_value)
{
    if (stack == NULL || out_value == NULL || stack->size == 0 || stack->items == NULL) {
        return 0;
    }

    stack->size--;
    *out_value = stack->items[stack->size];
    return 1;
}

int stack_peek(const GitLiteStack *stack, int *out_value)
{
    if (stack == NULL || out_value == NULL || stack->size == 0 || stack->items == NULL) {
        return 0;
    }

    *out_value = stack->items[stack->size - 1];
    return 1;
}

int stack_is_empty(const GitLiteStack *stack)
{
    return stack == NULL || stack->size == 0;
}

size_t stack_size(const GitLiteStack *stack)
{
    return stack == NULL ? 0 : stack->size;
}

void stack_clear(GitLiteStack *stack)
{
    if (stack != NULL) {
        stack->size = 0;
    }
}

void stack_destroy(GitLiteStack *stack)
{
    if (stack != NULL) {
        free(stack->items);
        stack->items = NULL;
        stack->size = 0;
        stack->capacity = 0;
    }
}