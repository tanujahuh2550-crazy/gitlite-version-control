
#include "commit.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    CommitHistory *history = init_commit_history();
    UndoStack *stack = init_undo_stack();

    assert(history != NULL);
    assert(stack != NULL);

    // Test 1: Create a commit
    int id = make_commit(history, stack,
                         "Initial commit", "Tanuja", NULL, 0);

    assert(id == 1);
    assert(history->total_commits == 1);
    assert(history->head != NULL);
    assert(strcmp(history->head->message, "Initial commit") == 0);
    printf("Commit creation test passed!\n");

    // Test 2: Add a tracked file through a new commit
    TrackedFile files[1] = {0};

    strcpy(files[0].filename, "main.c");
    strcpy(files[0].content, "int main() { return 0; }");

    id = make_commit(history, stack,
                     "Added main file", "Tanuja", files, 1);

    assert(id == 2);
    assert(history->total_commits == 2);
    assert(history->tail->file_count == 1);
    assert(strcmp(history->tail->files[0].filename, "main.c") == 0);
    assert(strcmp(history->tail->files[0].content,
                  "int main() { return 0; }") == 0);
    printf("Tracked file test passed!\n");

    // Test 3: Find commits
    assert(find_commit_by_id(history, 1) == history->head);
    assert(find_commit_by_id(history, 2) == history->tail);
    assert(find_commit_by_id(history, 99) == NULL);
    printf("Commit search test passed!\n");

    // Test 4: Undo stack
    assert(pop_undo(stack) == history->tail);
    printf("Undo stack pop test passed!\n");

    // Restore undo stack for cleanup
    assert(rebuild_undo_stack(stack, history) == 1);

    // Display history
    display_history(history);

    free_undo_stack(stack);
    free_commit_history(history);

    printf("\nAll basic Commit tests passed!\n");
    return 0;
}
