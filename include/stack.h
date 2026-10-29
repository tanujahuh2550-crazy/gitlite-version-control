#ifndef GITLITE_STACK_H
#define GITLITE_STACK_H

#include <stddef.h>

/* Dynamic array stack storing commit IDs (or other integer values). */
typedef struct {
    int *items;
    size_t size;
    size_t capacity;
} GitLiteStack;

/* Initialize before using any other stack function. Returns 1 on success. */
int stack_init(GitLiteStack *stack);

/* Push a value onto the top. Returns 1 on success, 0 on invalid input/allocation failure. */
int stack_push(GitLiteStack *stack, int value);

/* Pop the top value into out_value. Returns 1 on success, 0 if empty/invalid. */
int stack_pop(GitLiteStack *stack, int *out_value);

/* Read the top value without removing it. Returns 1 on success, 0 if empty/invalid. */
int stack_peek(const GitLiteStack *stack, int *out_value);

int stack_is_empty(const GitLiteStack *stack);
size_t stack_size(const GitLiteStack *stack);
void stack_clear(GitLiteStack *stack);
void stack_destroy(GitLiteStack *stack);

#endif /* GITLITE_STACK_H */