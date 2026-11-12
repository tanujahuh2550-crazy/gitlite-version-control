
#include "commit.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    CommitHistory *history = init_commit_history();
    CommitHistory *loaded = init_commit_history();
    UndoStack *stack = init_undo_stack();

    assert(history != NULL);
    assert(loaded != NULL);
    assert(stack != NULL);

    TrackedFile files[1] = {0};

    strcpy(files[0].filename, "main.c");
    strcpy(files[0].content, "Hello GitLite!");

    assert(make_commit(history, stack,
                       "Initial commit", "Tanuja", files, 1) == 1);

    assert(make_commit(history, stack,
                       "Second commit", "Kavya", NULL, 0) == 2);

    // Save history
    assert(save_history_to_file(history, "test_history.txt") == 1);
    printf("Save history test passed!\n");

    // Load history into a fresh history object
    assert(load_history_from_file(loaded, "test_history.txt") == 1);

    assert(loaded->total_commits == 2);
    assert(loaded->head->commitID == 1);
    assert(loaded->tail->commitID == 2);

    printf("Load history test passed!\n");

    // Verify tracked file data survived
    assert(loaded->head->file_count == 1);
    assert(strcmp(loaded->head->files[0].filename, "main.c") == 0);
    assert(strcmp(loaded->head->files[0].content, "Hello GitLite!") == 0);

    printf("Tracked file persistence test passed!\n");

    // Verify next commit ID
    assert(loaded->next_commit_id == 3);
    printf("Next commit ID test passed!\n");

    display_history(loaded);

    remove("test_history.txt");

    free_undo_stack(stack);
    free_commit_history(history);
    free_commit_history(loaded);

    printf("\nAll Persistence tests passed!\n");
    return 0;
}
