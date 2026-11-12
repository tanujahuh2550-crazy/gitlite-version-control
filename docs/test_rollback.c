
#include "commit.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    CommitHistory *history = init_commit_history();
    UndoStack *stack = init_undo_stack();

    assert(history != NULL);
    assert(stack != NULL);

    // Create three commits
    assert(make_commit(history, stack, "First", "Tanuja", NULL, 0) == 1);
    assert(make_commit(history, stack, "Second", "Tanuja", NULL, 0) == 2);
    assert(make_commit(history, stack, "Third", "Tanuja", NULL, 0) == 3);

    assert(history->total_commits == 3);
    printf("Three commits created successfully!\n");

    // Rollback latest commit
    assert(rollback_commit(history, stack) == 1);

    assert(history->total_commits == 2);
    assert(history->tail->commitID == 2);
    assert(history->tail->next == NULL);
    assert(find_commit_by_id(history, 3) == NULL);

    printf("First rollback test passed!\n");

    // Rollback another commit
    assert(rollback_commit(history, stack) == 1);

    assert(history->total_commits == 1);
    assert(history->head == history->tail);
    assert(history->head->commitID == 1);

    printf("Second rollback test passed!\n");

    // Check that next ID is not reused
    assert(history->next_commit_id == 4);

    assert(make_commit(history, stack, "Fourth", "Tanuja", NULL, 0) == 4);

    printf("Commit ID continuity test passed!\n");

    display_history(history);

    free_undo_stack(stack);
    free_commit_history(history);

    printf("\nAll Rollback tests passed!\n");
    return 0;
}
