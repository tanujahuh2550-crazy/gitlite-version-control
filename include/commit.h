#ifndef COMMIT_H
#define COMMIT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Fixed-size limits used by this educational implementation. */
#define MAX_FILES 10
#define MAX_UNDO 50
#define MAX_FILENAME_LENGTH 128
#define MAX_FILE_CONTENT_LENGTH 512
#define MAX_COMMIT_MESSAGE_LENGTH 256
#define MAX_AUTHOR_LENGTH 64
#define MAX_TIMESTAMP_LENGTH 32

typedef struct {
    char filename[MAX_FILENAME_LENGTH];
    char content[MAX_FILE_CONTENT_LENGTH];
} TrackedFile;

typedef struct Commit {
    int commitID;
    char message[MAX_COMMIT_MESSAGE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    char timestamp[MAX_TIMESTAMP_LENGTH];
    TrackedFile files[MAX_FILES];
    int file_count;
    struct Commit *next;
} Commit;

typedef struct {
    Commit *head;
    Commit *tail;
    int total_commits;
    /* Monotonically increasing ID; rollback does not decrement this value. */
    int next_commit_id;
} CommitHistory;

typedef struct {
    Commit *items[MAX_UNDO];
    int top;
} UndoStack;

/* Allocation and initialization. */
CommitHistory *init_commit_history(void);
UndoStack *init_undo_stack(void);

/* Commit and tracked-file management. */
Commit *create_commit_node(int id, const char *message, const char *author);
int add_file_to_commit(Commit *commit, const char *filename, const char *content);
int make_commit(CommitHistory *history, UndoStack *stack,
                const char *message, const char *author,
                TrackedFile files[], int file_count);

/* Stack operations. The stack retains the most recent MAX_UNDO commits. */
void push_undo(UndoStack *stack, Commit *commit);
Commit *pop_undo(UndoStack *stack);
int rebuild_undo_stack(UndoStack *stack, CommitHistory *history);

/* History operations. */
void display_history(const CommitHistory *history);
const Commit *find_commit_by_id(const CommitHistory *history, int commit_id);
int rollback_commit(CommitHistory *history, UndoStack *stack);

/* File persistence. The versioned text format hex-encodes stored text fields. */
int save_history_to_file(const CommitHistory *history, const char *filepath);
int load_history_from_file(CommitHistory *history, const char *filepath);

/* Memory cleanup. The undo stack does not own the commit nodes it references. */
void free_commit_history(CommitHistory *history);
void free_undo_stack(UndoStack *stack);

#endif /* COMMIT_H */
