
#include "search.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    Commit c1 = {0};
    Commit c2 = {0};
    CommitHistory history = {0};

    c1.commitID = 1;
    strcpy(c1.message, "Added login feature");
    strcpy(c1.author, "Tanuja");
    strcpy(c1.timestamp, "2026-10-29");

    c2.commitID = 2;
    strcpy(c2.message, "Fixed search bug");
    strcpy(c2.author, "Kavya");
    strcpy(c2.timestamp, "2026-10-29");

    c1.next = &c2;
    history.head = &c1;
    history.tail = &c2;
    history.total_commits = 2;

    assert(search_commit_by_id(&history, 1) == &c1);
    assert(search_commit_by_id(&history, 99) == NULL);

    assert(search_commits_by_message(&history, "login") == 1);
    assert(search_commits_by_author(&history, "Kavya") == 1);
    assert(search_commits(&history, "search") == 1);
    assert(search_commits(&history, "Tanuja") == 1);

    puts("\nAll Search tests passed.");
    return 0;
}
