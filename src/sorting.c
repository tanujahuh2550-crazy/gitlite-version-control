#include <stdio.h>
#include "sorting.h"

/* =========================================================
   MERGE SORT
   ========================================================= */

/* Merge two sorted linked lists */
static Commit* merge(Commit* left, Commit* right)
{
    if (left == NULL)
        return right;

    if (right == NULL)
        return left;

    Commit* result;

    if (left->commitID <= right->commitID)
    {
        result = left;
        result->next = merge(left->next, right);
    }
    else
    {
        result = right;
        result->next = merge(left, right->next);
    }

    return result;
}

/* Split linked list into two halves */
static void split_list(
    Commit* source,
    Commit** front,
    Commit** back
)
{
    if (source == NULL || source->next == NULL)
    {
        *front = source;
        *back = NULL;
        return;
    }

    Commit* slow = source;
    Commit* fast = source->next;

    while (fast != NULL)
    {
        fast = fast->next;

        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front = source;
    *back = slow->next;

    slow->next = NULL;
}

/* Recursive Merge Sort */
static Commit* merge_sort_recursive(Commit* head)
{
    if (head == NULL || head->next == NULL)
        return head;

    Commit* left;
    Commit* right;

    split_list(head, &left, &right);

    left = merge_sort_recursive(left);
    right = merge_sort_recursive(right);

    return merge(left, right);
}

/* Public Merge Sort function */
void merge_sort_commits(CommitHistory* history)
{
    if (history == NULL || history->head == NULL)
        return;

    history->head = merge_sort_recursive(history->head);

    /* Update tail */
    history->tail = history->head;

    while (history->tail->next != NULL)
    {
        history->tail = history->tail->next;
    }

    printf("\nCommit history sorted using Merge Sort.\n");
}


/* =========================================================
   QUICK SORT
   ========================================================= */

/* Swap the data of two commits */
static void swap_commit_data(Commit* a, Commit* b)
{
    if (a == NULL || b == NULL)
        return;

    // Swap commit IDs
    int temp_id = a->commitID;
    a->commitID = b->commitID;
    b->commitID = temp_id;

    // Swap messages
    char temp_message[MAX_COMMIT_MESSAGE_LENGTH];
    strcpy(temp_message, a->message);
    strcpy(a->message, b->message);
    strcpy(b->message, temp_message);

    // Swap authors
    char temp_author[MAX_AUTHOR_LENGTH];
    strcpy(temp_author, a->author);
    strcpy(a->author, b->author);
    strcpy(b->author, temp_author);

    // Swap timestamps
    char temp_timestamp[MAX_TIMESTAMP_LENGTH];
    strcpy(temp_timestamp, a->timestamp);
    strcpy(a->timestamp, b->timestamp);
    strcpy(b->timestamp, temp_timestamp);

    // Swap tracked files
    TrackedFile temp_files[MAX_FILES];

    memcpy(temp_files, a->files, sizeof(temp_files));
    memcpy(a->files, b->files, sizeof(a->files));
    memcpy(b->files, temp_files, sizeof(b->files));

    // Swap file counts
    int temp_count = a->file_count;
    a->file_count = b->file_count;
    b->file_count = temp_count;
}

/* Partition linked list for Quick Sort */
static Commit* partition(Commit* low, Commit* high)
{
    int pivot = high->commitID;

    Commit* i = low;

    for (Commit* j = low; j != high; j = j->next)
    {
        if (j->commitID <= pivot)
        {
            swap_commit_data(i, j);
            i = i->next;
        }
    }

    swap_commit_data(i, high);

    return i;
}

/* Recursive Quick Sort */
static void quick_sort_recursive(
    Commit* low,
    Commit* high
)
{
    if (low == NULL || high == NULL || low == high)
        return;

    Commit* pivot = partition(low, high);

    /* Sort left side */
    if (pivot != low)
    {
        Commit* current = low;

        while (current->next != pivot)
        {
            current = current->next;
        }

        quick_sort_recursive(low, current);
    }

    /* Sort right side */
    if (pivot->next != NULL)
    {
        quick_sort_recursive(pivot->next, high);
    }
}

/* Public Quick Sort function */
void quick_sort_commits(CommitHistory* history)
{
    if (history == NULL || history->head == NULL)
        return;

    Commit* tail = history->head;

    while (tail->next != NULL)
    {
        tail = tail->next;
    }

    quick_sort_recursive(history->head, tail);

    history->tail = tail;

    printf("\nCommit history sorted using Quick Sort.\n");
}


/* =========================================================
   DISPLAY
   ========================================================= */

void display_sorted_commits(
    const CommitHistory* history
)
{
    if (history == NULL || history->head == NULL)
    {
        printf("\nNo commits available.\n");
        return;
    }

    printf("\n========== SORTED COMMIT HISTORY ==========\n");

    Commit* current = history->head;

    while (current != NULL)
    {
        printf("\nCommit ID : %d\n", current->commitID);
        printf("Author    : %s\n", current->author);
        printf("Timestamp : %s\n", current->timestamp);
        printf("Message   : %s\n", current->message);

        current = current->next;
    }

    printf("\n===========================================\n");
}