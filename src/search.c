#include "search.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Search for a commit by its commit ID.
 */
Commit* search_commit_by_id(const CommitHistory* history, int commit_id)
{
    if (history == NULL) {
        return NULL;
    }

    Commit* current = history->head;

    while (current != NULL) {
        if (current->commitID == commit_id) {
            return current;
        }

        current = current->next;
    }

    return NULL;
}


/*
 * Search commits by message keyword.
 */
int search_commits_by_message(const CommitHistory* history, const char* keyword)
{
    if (history == NULL || keyword == NULL) {
        return 0;
    }

    int count = 0;
    Commit* current = history->head;

    while (current != NULL) {

        if (strstr(current->message, keyword) != NULL) {

            printf("\nCommit ID : %d", current->commitID);
            printf("\nAuthor    : %s", current->author);
            printf("\nTimestamp : %s", current->timestamp);
            printf("\nMessage   : %s\n", current->message);

            count++;
        }

        current = current->next;
    }

    return count;
}


/*
 * Search commits by author.
 */
int search_commits_by_author(const CommitHistory* history, const char* author)
{
    if (history == NULL || author == NULL) {
        return 0;
    }

    int count = 0;
    Commit* current = history->head;

    while (current != NULL) {

        if (strcmp(current->author, author) == 0) {

            printf("\nCommit ID : %d", current->commitID);
            printf("\nAuthor    : %s", current->author);
            printf("\nTimestamp : %s", current->timestamp);
            printf("\nMessage   : %s\n", current->message);

            count++;
        }

        current = current->next;
    }

    return count;
}


/*
 * General search.
 *
 * The keyword is searched in:
 *   1. Commit ID
 *   2. Commit message
 *   3. Author
 */
int search_commits(const CommitHistory* history, const char* keyword)
{
    if (history == NULL || keyword == NULL) {
        return 0;
    }

    int count = 0;
    Commit* current = history->head;

    while (current != NULL) {

        char commit_id_string[20];

        snprintf(
            commit_id_string,
            sizeof(commit_id_string),
            "%d",
            current->commitID
        );

        if (strstr(commit_id_string, keyword) != NULL ||
            strstr(current->message, keyword) != NULL ||
            strstr(current->author, keyword) != NULL) {

            printf("\nCommit ID : %d", current->commitID);
            printf("\nAuthor    : %s", current->author);
            printf("\nTimestamp : %s", current->timestamp);
            printf("\nMessage   : %s\n", current->message);

            count++;
        }

        current = current->next;
    }

    return count;
}