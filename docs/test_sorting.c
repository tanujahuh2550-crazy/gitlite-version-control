
#include "sorting.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void setup_commit(Commit *c, int id, const char *message, const char *author)
{
    c->commitID = id;
    snprintf(c->message, sizeof(c->message), "%s", message);
    snprintf(c->author, sizeof(c->author), "%s", author);
    snprintf(c->timestamp, sizeof(c->timestamp), "2026-10-30");
    c->next = NULL;
}

int is_sorted(CommitHistory *history)
{
    Commit *current = history->head;

    while (current != NULL && current->next != NULL)
    {
        if (current->commitID > current->next->commitID)
            return 0;

        current = current->next;
    }

    return 1;
}

int main(void)
{
    Commit c1 = {0}, c2 = {0}, c3 = {0}, c4 = {0};

    setup_commit(&c1, 30, "Third commit", "Tanuja");
    setup_commit(&c2, 10, "First commit", "Kavya");
    setup_commit(&c3, 40, "Fourth commit", "Yashasvi");
    setup_commit(&c4, 20, "Second commit", "Kaushal");

    c1.next = &c2;
    c2.next = &c3;
    c3.next = &c4;

    CommitHistory history = {0};
    history.head = &c1;
    history.tail = &c4;
    history.total_commits = 4;

    printf("Testing Merge Sort...\n");

    merge_sort_commits(&history);

    assert(is_sorted(&history));
    assert(history.head->commitID == 10);
    assert(history.tail->commitID == 40);

    printf("Merge Sort passed!\n");

    /* Reset list for Quick Sort */
    setup_commit(&c1, 30, "Third commit", "Tanuja");
    setup_commit(&c2, 10, "First commit", "Kavya");
    setup_commit(&c3, 40, "Fourth commit", "Yashasvi");
    setup_commit(&c4, 20, "Second commit", "Kaushal");

    c1.next = &c2;
    c2.next = &c3;
    c3.next = &c4;

    history.head = &c1;
    history.tail = &c4;

    printf("\nTesting Quick Sort...\n");

    quick_sort_commits(&history);

    assert(is_sorted(&history));
    assert(history.head->commitID == 10);
    assert(history.tail->commitID == 40);

    assert(strcmp(history.head->author, "Kavya") == 0);
    assert(strcmp(history.head->message, "First commit") == 0);

    printf("Quick Sort passed!\n");

    puts("\nAll Sorting tests passed!");

    return 0;
}
