#ifndef COMMIT_H
#define COMMIT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_UNDO 100

// Commit Node (Singly Linked List element)
typedef struct Commit {
    int commitID;
    char message[256];
    char author[64];
    char timestamp[32];
    struct Commit* next;
} Commit;

// Commit History (Linked List controller)
typedef struct {
    Commit* head;
    Commit* tail;
    int total_commits;
} CommitHistory;

// Undo Stack (Array-based Stack for LIFO undo)
typedef struct {
    Commit* items[MAX_UNDO];
    int top;
} UndoStack;

// Core Function Declarations
CommitHistory* init_commit_history(void);
UndoStack* init_undo_stack(void);

Commit* create_commit_node(int id, const char* message, const char* author);
int make_commit(CommitHistory* history, UndoStack* stack, const char* message, const char* author);
void display_history(const CommitHistory* history);

// Stack & Undo Operations
void push_undo(UndoStack* stack, Commit* commit);
Commit* pop_undo(UndoStack* stack);
int rollback_commit(CommitHistory* history, UndoStack* stack);

// Memory Cleanup
void free_commit_history(CommitHistory* history);
void free_undo_stack(UndoStack* stack);

#endif // COMMIT_H