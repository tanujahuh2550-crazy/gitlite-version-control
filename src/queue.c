#include "queue.h"

#include <stdlib.h>
#include <string.h>

static int copy_checked(char *destination, size_t destination_size, const char *source,
                        int must_not_be_empty)
{
    size_t length;

    if (destination == NULL || source == NULL || destination_size == 0) {
        return 0;
    }

    length = strlen(source);
    if (length >= destination_size || (must_not_be_empty && length == 0)) {
        return 0; /* Reject oversized strings instead of silently truncating them. */
    }

    memcpy(destination, source, length + 1);
    return 1;
}

static int request_is_valid(const MergeRequest *request)
{
    return request != NULL && request->request_id > 0 &&
           memchr(request->source_branch, '\0', sizeof(request->source_branch)) != NULL &&
           memchr(request->target_branch, '\0', sizeof(request->target_branch)) != NULL &&
           memchr(request->description, '\0', sizeof(request->description)) != NULL &&
           request->source_branch[0] != '\0' && request->target_branch[0] != '\0';
}

int queue_init(GitLiteQueue *queue)
{
    if (queue == NULL) {
        return 0;
    }

    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return 1;
}

int queue_enqueue(GitLiteQueue *queue, const MergeRequest *request)
{
    GitLiteQueueNode *node;

    if (queue == NULL || !request_is_valid(request)) {
        return 0;
    }

    node = (GitLiteQueueNode *)malloc(sizeof(*node));
    if (node == NULL) {
        return 0;
    }

    node->request = *request;
    node->next = NULL;

    if (queue->rear == NULL) {
        /* First item: both ends point to the same node. */
        queue->front = node;
        queue->rear = node;
    } else {
        queue->rear->next = node;
        queue->rear = node;
    }

    queue->size++;
    return 1;
}

int queue_enqueue_details(GitLiteQueue *queue,
                          unsigned long request_id,
                          const char *source_branch,
                          const char *target_branch,
                          const char *description)
{
    MergeRequest request = {0};

    if (request_id == 0 ||
        !copy_checked(request.source_branch, sizeof(request.source_branch), source_branch, 1) ||
        !copy_checked(request.target_branch, sizeof(request.target_branch), target_branch, 1) ||
        !copy_checked(request.description, sizeof(request.description),
                      description == NULL ? "" : description, 0)) {
        return 0;
    }

    request.request_id = request_id;
    return queue_enqueue(queue, &request);
}


int queue_dequeue(GitLiteQueue *queue, MergeRequest *out_request)
{
    GitLiteQueueNode *old_front;

    if (queue == NULL || out_request == NULL || queue->front == NULL) {
        return 0;
    }

    old_front = queue->front;
    *out_request = old_front->request;

    queue->front = old_front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(old_front);
    queue->size--;

    return 1;
}

int queue_peek(const GitLiteQueue *queue, MergeRequest *out_request)
{
    if (queue == NULL || out_request == NULL || queue->front == NULL) {
        return 0;
    }

    *out_request = queue->front->request;
    return 1;
}

int queue_is_empty(const GitLiteQueue *queue)
{
    return queue == NULL || queue->front == NULL;
}

size_t queue_size(const GitLiteQueue *queue)
{
    return queue == NULL ? 0 : queue->size;
}

void queue_clear(GitLiteQueue *queue)
{
    GitLiteQueueNode *current;

    if (queue == NULL) {
        return;
    }

    current = queue->front;

    while (current != NULL) {
        GitLiteQueueNode *next = current->next;
        free(current);
        current = next;
    }

    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

void queue_destroy(GitLiteQueue *queue)
{
    queue_clear(queue);
}
