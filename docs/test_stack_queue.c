
#include "queue.h"
#include "stack.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void test_stack(void)
{
    GitLiteStack stack;
    int value = 0;
    int i;

    assert(stack_init(&stack) == 1);
    assert(stack_is_empty(&stack));
    assert(stack_pop(&stack, &value) == 0);

    assert(stack_push(&stack, 10) == 1);
    assert(stack_push(&stack, 20) == 1);
    assert(stack_push(&stack, 30) == 1);
    assert(stack_size(&stack) == 3);
    assert(stack_peek(&stack, &value) == 1 && value == 30);
    assert(stack_pop(&stack, &value) == 1 && value == 30);
    assert(stack_pop(&stack, &value) == 1 && value == 20);
    assert(stack_pop(&stack, &value) == 1 && value == 10);
    assert(stack_is_empty(&stack));

    for (i = 0; i < 1000; ++i) {
        assert(stack_push(&stack, i) == 1);
    }

    assert(stack_size(&stack) == 1000);

    for (i = 999; i >= 0; --i) {
        assert(stack_pop(&stack, &value) == 1 && value == i);
    }

    stack_clear(&stack);
    stack_destroy(&stack);
}

static void test_queue(void)
{
    GitLiteQueue queue;
    MergeRequest request;

    assert(queue_init(&queue) == 1);
    assert(queue_is_empty(&queue));
    assert(queue_dequeue(&queue, &request) == 0);

    assert(queue_enqueue_details(&queue, 101, "feature/login",
                                 "main", "Login implementation") == 1);
    assert(queue_enqueue_details(&queue, 102, "fix/typo",
                                 "main", "Fix documentation") == 1);
    assert(queue_enqueue_details(&queue, 103, "feature/search",
                                 "develop", "Search module") == 1);
    assert(queue_size(&queue) == 3);

    assert(queue_peek(&queue, &request) == 1);
    assert(request.request_id == 101);
    assert(strcmp(request.source_branch, "feature/login") == 0);

    assert(queue_dequeue(&queue, &request) == 1 && request.request_id == 101);
    assert(queue_dequeue(&queue, &request) == 1 && request.request_id == 102);
    assert(queue_dequeue(&queue, &request) == 1 && request.request_id == 103);
    assert(queue_is_empty(&queue));
    assert(queue_size(&queue) == 0);

    assert(queue_enqueue_details(&queue, 0, "feature/a", "main", "bad id") == 0);

    queue_destroy(&queue);
}

int main(void)
{
    test_stack();
    test_queue();
    puts("All Stack and Queue tests passed.");
    return 0;
}
