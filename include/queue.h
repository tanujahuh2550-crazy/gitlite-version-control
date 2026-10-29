#ifndef GITLITE_QUEUE_H
#define GITLITE_QUEUE_H

#include <stddef.h>

#define GITLITE_BRANCH_NAME_MAX 64
#define GITLITE_MERGE_DESCRIPTION_MAX 256

/* One merge request waiting to be handled, in FIFO order. */
typedef struct {
    unsigned long request_id;
    char source_branch[GITLITE_BRANCH_NAME_MAX];
    char target_branch[GITLITE_BRANCH_NAME_MAX];
    char description[GITLITE_MERGE_DESCRIPTION_MAX];
} MergeRequest;

typedef struct GitLiteQueueNode {
    MergeRequest request;
    struct GitLiteQueueNode *next;
} GitLiteQueueNode;

typedef struct {
    GitLiteQueueNode *front;
    GitLiteQueueNode *rear;
    size_t size;
} GitLiteQueue;

/* Initialize before using any other queue function. */
int queue_init(GitLiteQueue *queue);

/* Add a validated request at the rear. Returns 1 on success, otherwise 0. */
int queue_enqueue(GitLiteQueue *queue, const MergeRequest *request);

/* Convenience helper that builds a request safely from strings. */
int queue_enqueue_details(GitLiteQueue *queue,
                          unsigned long request_id,
                          const char *source_branch,
                          const char *target_branch,
                          const char *description);

/* Remove the oldest request into out_request. Returns 1 on success, 0 if empty/invalid. */
int queue_dequeue(GitLiteQueue *queue, MergeRequest *out_request);

/* Read the oldest request without removing it. */
int queue_peek(const GitLiteQueue *queue, MergeRequest *out_request);

int queue_is_empty(const GitLiteQueue *queue);
size_t queue_size(const GitLiteQueue *queue);
void queue_clear(GitLiteQueue *queue);
void queue_destroy(GitLiteQueue *queue);

#endif /* GITLITE_QUEUE_H */