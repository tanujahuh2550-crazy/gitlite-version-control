#include "../include/commit.h"

// Helper function to generate current timestamp
static void get_current_timestamp(char* buffer, size_t size) {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    if (timeinfo != NULL) {
        strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeinfo);
    } else {
        snprintf(buffer, size, "Unknown Time");
    }
}

// Initialize empty Commit History linked list
CommitHistory* init_commit_history(void) {
    CommitHistory* history = (CommitHistory*)malloc(sizeof(CommitHistory));
    if (!history) return NULL;
    history->head = NULL;
    history->tail = NULL;
    history->total_commits = 0;
    return history;
}

// Initialize empty Undo Stack
UndoStack* init_undo_stack(void) {
    UndoStack* stack = (UndoStack*)malloc(sizeof(UndoStack));
    if (!stack) return NULL;
    stack->top = -1;
    return stack;
}

// Create a new Commit Node safely
Commit* create_commit_node(int id, const char* message, const char* author) {
    Commit* new_node = (Commit*)malloc(sizeof(Commit));
    if (!new_node) return NULL;

    new_node->commitID = id;

    // Safe string copies with null termination
    strncpy(new_node->message, message ? message : "", sizeof(new_node->message) - 1);
    new_node->message[sizeof(new_node->message) - 1] = '\0';

    strncpy(new_node->author, author ? author : "", sizeof(new_node->author) - 1);
    new_node->author[sizeof(new_node->author) - 1] = '\0';

    get_current_timestamp(new_node->timestamp, sizeof(new_node->timestamp));
    new_node->file_count = 0;
    new_node->next = NULL;

    return new_node;
}

// Add tracked file snapshot to commit node
int add_file_to_commit(Commit* commit, const char* filename, const char* content) {
    if (!commit || commit->file_count >= MAX_FILES || !filename) return 0;

    int idx = commit->file_count;
    strncpy(commit->files[idx].filename, filename, sizeof(commit->files[idx].filename) - 1);
    commit->files[idx].filename[sizeof(commit->files[idx].filename) - 1] = '\0';

    strncpy(commit->files[idx].content, content ? content : "", sizeof(commit->files[idx].content) - 1);
    commit->files[idx].content[sizeof(commit->files[idx].content) - 1] = '\0';

    commit->file_count++;
    return 1;
}

// Push commit reference onto Undo Stack
void push_undo(UndoStack* stack, Commit* commit) {
    if (!stack || !commit) return;
    if (stack->top >= MAX_UNDO - 1) {
        printf("Warning: Undo stack memory limit reached.\n");
        return;
    }
    stack->items[++(stack->top)] = commit;
}

// Pop commit reference from Undo Stack
Commit* pop_undo(UndoStack* stack) {
    if (!stack || stack->top < 0) return NULL;
    return stack->items[(stack->top)--];
}

// Create and insert commit into linked list and stack
int make_commit(CommitHistory* history, UndoStack* stack, const char* message, const char* author, TrackedFile files[], int file_count) {
    if (!history || !stack) return 0;

    int new_id = history->total_commits + 1;
    Commit* new_commit = create_commit_node(new_id, message, author);
    if (!new_commit) return 0;

    // Attach tracked files
    if (files != NULL && file_count > 0) {
        for (int i = 0; i < file_count && i < MAX_FILES; i++) {
            add_file_to_commit(new_commit, files[i].filename, files[i].content);
        }
    }

    // Append to linked list tail
    if (history->head == NULL) {
        history->head = new_commit;
        history->tail = new_commit;
    } else {
        history->tail->next = new_commit;
        history->tail = new_commit;
    }
    history->total_commits++;

    // Push to undo stack
    push_undo(stack, new_commit);
    return new_id;
}

// Traverse and print commit history
void display_history(const CommitHistory* history) {
    if (!history || history->head == NULL) {
        printf("\nNo commit history available.\n");
        return;
    }

    printf("\n-----------------------------------------\n");
    printf("           GITLITE COMMIT LOG           \n");
    printf("-----------------------------------------\n");

    Commit* current = history->head;
    while (current != NULL) {
        printf("Commit ID : #%d\n", current->commitID);
        printf("Author    : %s\n", current->author);
        printf("Date      : %s\n", current->timestamp);
        printf("Message   : %s\n", current->message);
        printf("Files     : %d file(s) tracked\n", current->file_count);

        for (int i = 0; i < current->file_count; i++) {
            printf("  - %s\n", current->files[i].filename);
        }
        printf("-----------------------------------------\n");
        current = current->next;
    }
}

// Roll back the latest commit
int rollback_commit(CommitHistory* history, UndoStack* stack) {
    if (!history || !stack || history->head == NULL) {
        printf("Nothing to undo.\n");
        return 0;
    }

    Commit* popped = pop_undo(stack);
    if (!popped) return 0;

    int id_undone = popped->commitID;

    if (history->head == history->tail) {
        free(history->head);
        history->head = NULL;
        history->tail = NULL;
    } else {
        Commit* prev = history->head;
        while (prev->next != history->tail) {
            prev = prev->next;
        }
        free(history->tail);
        prev->next = NULL;
        history->tail = prev;
    }

    history->total_commits--;
    printf("Rollback complete: Commit #%d removed.\n", id_undone);
    return 1;
}

// Save linked list to text file
int save_history_to_file(const CommitHistory* history, const char* filepath) {
    if (!history || !filepath) return 0;

    FILE* fp = fopen(filepath, "w");
    if (!fp) return 0;

    Commit* current = history->head;
    while (current != NULL) {
        fprintf(fp, "%d;%s;%s;%s;%d\n",
                current->commitID,
                current->author,
                current->timestamp,
                current->message,
                current->file_count);

        for (int i = 0; i < current->file_count; i++) {
            fprintf(fp, "%s;%s\n", current->files[i].filename, current->files[i].content);
        }
        current = current->next;
    }

    fclose(fp);
    return 1;
}

// Load commits from file into linked list
int load_history_from_file(CommitHistory* history, const char* filepath) {
    if (!history || !filepath) return 0;

    FILE* fp = fopen(filepath, "r");
    if (!fp) return 0;

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        int id, file_cnt;
        char author[64], timestamp[32], message[256];

        if (sscanf(line, "%d;%63[^;];%31[^;];%255[^;];%d", &id, author, timestamp, message, &file_cnt) == 5) {
            Commit* node = create_commit_node(id, message, author);
            if (!node) break;

            strncpy(node->timestamp, timestamp, sizeof(node->timestamp) - 1);

            for (int i = 0; i < file_cnt; i++) {
                if (fgets(line, sizeof(line), fp)) {
                    char fname[128], fcontent[512];
                    if (sscanf(line, "%127[^;];%511[^\n]", fname, fcontent) >= 1) {
                        add_file_to_commit(node, fname, fcontent);
                    }
                }
            }

            if (history->head == NULL) {
                history->head = node;
                history->tail = node;
            } else {
                history->tail->next = node;
                history->tail = node;
            }
            history->total_commits++;
        }
    }

    fclose(fp);
    return 1;
}

// Free history linked list memory
void free_commit_history(CommitHistory* history) {
    if (!history) return;
    Commit* current = history->head;
    while (current != NULL) {
        Commit* temp = current;
        current = current->next;
        free(temp);
    }
    free(history);
}

// Free stack memory
void free_undo_stack(UndoStack* stack) {
    if (stack) {
        free(stack);
    }
}