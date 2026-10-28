#include "commit.h"

#include <errno.h>
#include <limits.h>

#define SERIALIZED_LINE_CAP 2048
#define HISTORY_FILE_MAGIC "GITLITE_COMMIT_HISTORY;1;"

static void get_current_timestamp(char *buffer, size_t size)
{
    time_t rawtime;
    struct tm *timeinfo;

    if (buffer == NULL || size == 0) {
        return;
    }

    (void)snprintf(buffer, size, "%s", "Unknown Time");
    rawtime = time(NULL);
    timeinfo = localtime(&rawtime);
    if (timeinfo != NULL) {
        if (strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeinfo) == 0) {
            (void)snprintf(buffer, size, "%s", "Unknown Time");
        }
    }
}

static void strip_line_ending(char *text)
{
    size_t length;

    if (text == NULL) {
        return;
    }

    length = strlen(text);
    while (length > 0 && (text[length - 1] == '\n' || text[length - 1] == '\r')) {
        text[--length] = '\0';
    }
}

/* Copy only if the complete string fits; silently truncating a commit is unsafe. */
static int copy_text(char *destination, size_t capacity, const char *source)
{
    size_t length;

    if (destination == NULL || capacity == 0 || source == NULL) {
        return 0;
    }

    length = strlen(source);
    if (length >= capacity) {
        return 0;
    }

    memcpy(destination, source, length + 1);
    return 1;
}

/* Reads a delimiter-terminated field without silently truncating it. */
static int take_field(char **cursor, char *output, size_t capacity, char delimiter)
{
    char *end;
    size_t length;

    if (cursor == NULL || *cursor == NULL || output == NULL || capacity == 0) {
        return 0;
    }

    end = strchr(*cursor, delimiter);
    if (end == NULL) {
        return 0;
    }

    length = (size_t)(end - *cursor);
    if (length >= capacity) {
        return 0;
    }

    memcpy(output, *cursor, length);
    output[length] = '\0';
    *cursor = end + 1;
    return 1;
}

static int take_remaining(char **cursor, char *output, size_t capacity)
{
    size_t length;

    if (cursor == NULL || *cursor == NULL || output == NULL || capacity == 0) {
        return 0;
    }

    length = strlen(*cursor);
    if (length >= capacity) {
        return 0;
    }

    memcpy(output, *cursor, length + 1);
    *cursor += length;
    return 1;
}

static int hex_value(char character)
{
    if (character >= '0' && character <= '9') {
        return character - '0';
    }
    if (character >= 'a' && character <= 'f') {
        return character - 'a' + 10;
    }
    if (character >= 'A' && character <= 'F') {
        return character - 'A' + 10;
    }
    return -1;
}

/* Hex encoding keeps semicolons and line breaks inside text fields safe to save. */
static int write_hex_field(FILE *file, const char *text)
{
    static const char digits[] = "0123456789ABCDEF";
    const unsigned char *cursor = (const unsigned char *)text;

    if (file == NULL || text == NULL) {
        return 0;
    }

    while (*cursor != '\0') {
        if (fputc(digits[*cursor >> 4], file) == EOF ||
            fputc(digits[*cursor & 0x0F], file) == EOF) {
            return 0;
        }
        ++cursor;
    }

    return 1;
}

static int decode_hex_field(const char *encoded, char *output, size_t capacity)
{
    size_t encoded_length;
    size_t i;

    if (encoded == NULL || output == NULL || capacity == 0) {
        return 0;
    }

    encoded_length = strlen(encoded);
    if ((encoded_length % 2) != 0 || (encoded_length / 2) >= capacity) {
        return 0;
    }

    for (i = 0; i < encoded_length; i += 2) {
        int high = hex_value(encoded[i]);
        int low = hex_value(encoded[i + 1]);
        unsigned char value;

        if (high < 0 || low < 0) {
            return 0;
        }

        value = (unsigned char)((high << 4) | low);
        /* These fields are C strings, so embedded NUL bytes are not supported. */
        if (value == '\0') {
            return 0;
        }
        output[i / 2] = (char)value;
    }

    output[encoded_length / 2] = '\0';
    return 1;
}

static void free_commit_nodes(Commit *head)
{
    while (head != NULL) {
        Commit *next = head->next;
        free(head);
        head = next;
    }
}

static int parse_long_exact(const char *text, long minimum, long maximum, long *value_out)
{
    char *end;
    long value;

    if (text == NULL || *text == '\0' || value_out == NULL) {
        return 0;
    }

    errno = 0;
    value = strtol(text, &end, 10);
    if (errno == ERANGE || end == text || *end != '\0' ||
        value < minimum || value > maximum) {
        return 0;
    }

    *value_out = value;
    return 1;
}

static int append_node(CommitHistory *history, Commit *node)
{
    if (history == NULL || node == NULL || history->total_commits == INT_MAX) {
        return 0;
    }

    if (history->head == NULL) {
        history->head = node;
        history->tail = node;
    } else {
        history->tail->next = node;
        history->tail = node;
    }
    history->total_commits++;
    return 1;
}

CommitHistory *init_commit_history(void)
{
    CommitHistory *history = (CommitHistory *)malloc(sizeof(*history));

    if (history == NULL) {
        return NULL;
    }

    history->head = NULL;
    history->tail = NULL;
    history->total_commits = 0;
    history->next_commit_id = 1;
    return history;
}

UndoStack *init_undo_stack(void)
{
    UndoStack *stack = (UndoStack *)malloc(sizeof(*stack));
    int i;

    if (stack == NULL) {
        return NULL;
    }

    for (i = 0; i < MAX_UNDO; ++i) {
        stack->items[i] = NULL;
    }
    stack->top = -1;
    return stack;
}

Commit *create_commit_node(int id, const char *message, const char *author)
{
    Commit *node;

    if (id <= 0 || message == NULL || author == NULL ||
        strlen(message) >= MAX_COMMIT_MESSAGE_LENGTH ||
        strlen(author) >= MAX_AUTHOR_LENGTH) {
        return NULL;
    }

    node = (Commit *)malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }

    node->commitID = id;
    (void)copy_text(node->message, sizeof(node->message), message);
    (void)copy_text(node->author, sizeof(node->author), author);
    get_current_timestamp(node->timestamp, sizeof(node->timestamp));
    node->file_count = 0;
    node->next = NULL;
    return node;
}

int add_file_to_commit(Commit *commit, const char *filename, const char *content)
{
    int i;
    int index;

    if (commit == NULL || filename == NULL || filename[0] == '\0' ||
        (content == NULL ? 0U : strlen(content)) >= MAX_FILE_CONTENT_LENGTH ||
        strlen(filename) >= MAX_FILENAME_LENGTH ||
        commit->file_count < 0 || commit->file_count >= MAX_FILES) {
        return 0;
    }

    for (i = 0; i < commit->file_count; ++i) {
        if (strcmp(commit->files[i].filename, filename) == 0) {
            return 0; /* A snapshot should contain each filename at most once. */
        }
    }

    index = commit->file_count;
    if (!copy_text(commit->files[index].filename,
                   sizeof(commit->files[index].filename), filename) ||
        !copy_text(commit->files[index].content,
                   sizeof(commit->files[index].content), content == NULL ? "" : content)) {
        return 0;
    }

    commit->file_count++;
    return 1;
}

void push_undo(UndoStack *stack, Commit *commit)
{
    if (stack == NULL || commit == NULL) {
        return;
    }

    /* Keep the newest MAX_UNDO references instead of rejecting new commits. */
    if (stack->top >= MAX_UNDO - 1) {
        memmove(&stack->items[0], &stack->items[1],
                sizeof(stack->items[0]) * (MAX_UNDO - 1));
        stack->top = MAX_UNDO - 2;
    }

    stack->items[++stack->top] = commit;
}

Commit *pop_undo(UndoStack *stack)
{
    Commit *commit;

    if (stack == NULL || stack->top < 0) {
        return NULL;
    }

    commit = stack->items[stack->top];
    stack->items[stack->top] = NULL;
    stack->top--;
    return commit;
}

int rebuild_undo_stack(UndoStack *stack, CommitHistory *history)
{
    Commit *current;
    int i;

    if (stack == NULL || history == NULL) {
        return 0;
    }

    for (i = 0; i < MAX_UNDO; ++i) {
        stack->items[i] = NULL;
    }
    stack->top = -1;

    current = history->head;
    while (current != NULL) {
        push_undo(stack, current);
        current = current->next;
    }

    return 1;
}

int make_commit(CommitHistory *history, UndoStack *stack,
                const char *message, const char *author,
                TrackedFile files[], int file_count)
{
    Commit *new_commit;
    int i;
    int new_id;

    if (history == NULL || stack == NULL || message == NULL || author == NULL ||
        file_count < 0 || file_count > MAX_FILES ||
        (file_count > 0 && files == NULL) ||
        history->total_commits == INT_MAX ||
        history->next_commit_id <= 0 || history->next_commit_id >= INT_MAX) {
        return 0;
    }

    if (strlen(message) >= MAX_COMMIT_MESSAGE_LENGTH ||
        strlen(author) >= MAX_AUTHOR_LENGTH) {
        return 0;
    }

    /* Validate file data before allocating or modifying the history. */
    for (i = 0; i < file_count; ++i) {
        int j;
        if (memchr(files[i].filename, '\0', sizeof(files[i].filename)) == NULL ||
            memchr(files[i].content, '\0', sizeof(files[i].content)) == NULL ||
            files[i].filename[0] == '\0' ||
            strlen(files[i].filename) >= MAX_FILENAME_LENGTH ||
            strlen(files[i].content) >= MAX_FILE_CONTENT_LENGTH) {
            return 0;
        }
        for (j = 0; j < i; ++j) {
            if (strcmp(files[i].filename, files[j].filename) == 0) {
                return 0;
            }
        }
    }

    new_id = history->next_commit_id;
    new_commit = create_commit_node(new_id, message, author);
    if (new_commit == NULL) {
        return 0;
    }

    for (i = 0; i < file_count; ++i) {
        if (!add_file_to_commit(new_commit, files[i].filename, files[i].content)) {
            free(new_commit);
            return 0;
        }
    }

    if (!append_node(history, new_commit)) {
        free(new_commit);
        return 0;
    }

    history->next_commit_id = new_id + 1;
    push_undo(stack, new_commit);
    return new_id;
}

void display_history(const CommitHistory *history)
{
    const Commit *current;
    int i;

    if (history == NULL || history->head == NULL) {
        printf("\nNo commit history available.\n");
        return;
    }

    printf("\n=========================================\n");
    printf("             GITLITE COMMIT LOG          \n");
    printf("=========================================\n");

    current = history->head;
    while (current != NULL) {
        printf("Commit ID : #%d\n", current->commitID);
        printf("Author    : %s\n", current->author);
        printf("Date      : %s\n", current->timestamp);
        printf("Message   : %s\n", current->message);
        printf("Files     : %d file(s) in snapshot\n", current->file_count);

        for (i = 0; i < current->file_count; ++i) {
            printf("  - %s\n", current->files[i].filename);
        }
        printf("-----------------------------------------\n");
        current = current->next;
    }
    printf("Total commits: %d\n", history->total_commits);
}

const Commit *find_commit_by_id(const CommitHistory *history, int commit_id)
{
    const Commit *current;

    if (history == NULL || commit_id <= 0) {
        return NULL;
    }

    current = history->head;
    while (current != NULL) {
        if (current->commitID == commit_id) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

int rollback_commit(CommitHistory *history, UndoStack *stack)
{
    Commit *removed;
    Commit *previous;
    int removed_id;

    if (history == NULL || stack == NULL || history->head == NULL) {
        printf("Nothing to undo.\n");
        return 0;
    }

    if (stack->top < 0) {
        printf("No undoable commits remain in the stack.\n");
        return 0;
    }

    /* Verify consistency before changing either data structure. */
    if (stack->items[stack->top] != history->tail) {
        printf("Undo failed: undo stack is out of sync with commit history.\n");
        return 0;
    }

    removed = pop_undo(stack);
    if (removed == NULL) {
        printf("Nothing to undo.\n");
        return 0;
    }

    removed_id = removed->commitID;
    if (history->head == history->tail) {
        history->head = NULL;
        history->tail = NULL;
    } else {
        previous = history->head;
        while (previous->next != history->tail) {
            previous = previous->next;
        }
        previous->next = NULL;
        history->tail = previous;
    }

    free(removed);
    history->total_commits--;
    printf("Rollback complete: Commit #%d removed from history.\n", removed_id);
    printf("Note: this removes the commit record; it does not rewrite files on disk.\n");
    return 1;
}

int save_history_to_file(const CommitHistory *history, const char *filepath)
{
    FILE *file;
    const Commit *current;
    int i;
    int success = 1;

    if (history == NULL || filepath == NULL || filepath[0] == '\0' ||
        history->next_commit_id <= 0) {
        return 0;
    }

    file = fopen(filepath, "w");
    if (file == NULL) {
        return 0;
    }

    if (fprintf(file, HISTORY_FILE_MAGIC "%d\n", history->next_commit_id) < 0) {
        success = 0;
    }

    current = history->head;
    while (success && current != NULL) {
        if (current->file_count < 0 || current->file_count > MAX_FILES ||
            fprintf(file, "C;%d;%d;", current->commitID, current->file_count) < 0 ||
            !write_hex_field(file, current->author) || fputc(';', file) == EOF ||
            !write_hex_field(file, current->timestamp) || fputc(';', file) == EOF ||
            !write_hex_field(file, current->message) || fputc('\n', file) == EOF) {
            success = 0;
            break;
        }

        for (i = 0; success && i < current->file_count; ++i) {
            if (fprintf(file, "F;") < 0 ||
                !write_hex_field(file, current->files[i].filename) || fputc(';', file) == EOF ||
                !write_hex_field(file, current->files[i].content) || fputc('\n', file) == EOF) {
                success = 0;
            }
        }

        current = current->next;
    }

    if (ferror(file)) {
        success = 0;
    }
    if (fclose(file) != 0) {
        success = 0;
    }

    return success;
}

int load_history_from_file(CommitHistory *history, const char *filepath)
{
    FILE *file = NULL;
    char line[SERIALIZED_LINE_CAP];
    char *cursor;
    long header_next_id;
    long parsed_id;
    long parsed_file_count;
    long max_id = 0;
    CommitHistory loaded = {NULL, NULL, 0, 1};
    int success = 0;

    if (history == NULL || filepath == NULL || filepath[0] == '\0' ||
        history->head != NULL || history->tail != NULL || history->total_commits != 0) {
        return 0; /* Load into an empty history; callers decide how to replace active data. */
    }

    file = fopen(filepath, "r");
    if (file == NULL) {
        return 0;
    }

    if (fgets(line, sizeof(line), file) == NULL) {
        goto cleanup;
    }
    strip_line_ending(line);
    if (strncmp(line, HISTORY_FILE_MAGIC, strlen(HISTORY_FILE_MAGIC)) != 0 ||
        !parse_long_exact(line + strlen(HISTORY_FILE_MAGIC), 1, INT_MAX, &header_next_id)) {
        goto cleanup;
    }
    loaded.next_commit_id = (int)header_next_id;

    while (fgets(line, sizeof(line), file) != NULL) {
        char id_buffer[32];
        char count_buffer[32];
        char author_hex[(MAX_AUTHOR_LENGTH * 2) + 1];
        char timestamp_hex[(MAX_TIMESTAMP_LENGTH * 2) + 1];
        char message_hex[(MAX_COMMIT_MESSAGE_LENGTH * 2) + 1];
        char author[MAX_AUTHOR_LENGTH];
        char timestamp[MAX_TIMESTAMP_LENGTH];
        char message[MAX_COMMIT_MESSAGE_LENGTH];
        Commit *node = NULL;
        int i;

        strip_line_ending(line);
        if (line[0] == '\0') {
            continue;
        }
        if (strncmp(line, "C;", 2) != 0) {
            goto cleanup;
        }

        cursor = line + 2;
        if (!take_field(&cursor, id_buffer, sizeof(id_buffer), ';') ||
            !take_field(&cursor, count_buffer, sizeof(count_buffer), ';') ||
            !take_field(&cursor, author_hex, sizeof(author_hex), ';') ||
            !take_field(&cursor, timestamp_hex, sizeof(timestamp_hex), ';') ||
            !take_remaining(&cursor, message_hex, sizeof(message_hex)) ||
            !parse_long_exact(id_buffer, 1, INT_MAX, &parsed_id) ||
            !parse_long_exact(count_buffer, 0, MAX_FILES, &parsed_file_count) ||
            !decode_hex_field(author_hex, author, sizeof(author)) ||
            !decode_hex_field(timestamp_hex, timestamp, sizeof(timestamp)) ||
            !decode_hex_field(message_hex, message, sizeof(message))) {
            goto cleanup;
        }

        if (parsed_id == INT_MAX || loaded.total_commits == INT_MAX) {
            goto cleanup; /* Keep room for a valid next ID and avoid count overflow. */
        }

        {
            const Commit *existing = loaded.head;
            while (existing != NULL) {
                if (existing->commitID == (int)parsed_id) {
                    goto cleanup; /* Duplicate commit IDs are invalid. */
                }
                existing = existing->next;
            }
        }

        node = create_commit_node((int)parsed_id, message, author);
        if (node == NULL || !copy_text(node->timestamp, sizeof(node->timestamp), timestamp)) {
            free(node);
            goto cleanup;
        }

        for (i = 0; i < (int)parsed_file_count; ++i) {
            char file_line[SERIALIZED_LINE_CAP];
            char filename_hex[(MAX_FILENAME_LENGTH * 2) + 1];
            char content_hex[(MAX_FILE_CONTENT_LENGTH * 2) + 1];
            char filename[MAX_FILENAME_LENGTH];
            char content[MAX_FILE_CONTENT_LENGTH];
            char *file_cursor;

            if (fgets(file_line, sizeof(file_line), file) == NULL) {
                free(node);
                goto cleanup;
            }
            strip_line_ending(file_line);
            if (strncmp(file_line, "F;", 2) != 0) {
                free(node);
                goto cleanup;
            }

            file_cursor = file_line + 2;
            if (!take_field(&file_cursor, filename_hex, sizeof(filename_hex), ';') ||
                !take_remaining(&file_cursor, content_hex, sizeof(content_hex)) ||
                !decode_hex_field(filename_hex, filename, sizeof(filename)) ||
                !decode_hex_field(content_hex, content, sizeof(content)) ||
                !add_file_to_commit(node, filename, content)) {
                free(node);
                goto cleanup;
            }
        }

        if (!append_node(&loaded, node)) {
            free(node);
            goto cleanup;
        }
        if (parsed_id > max_id) {
            max_id = parsed_id;
        }
    }

    if (ferror(file)) {
        goto cleanup;
    }

    if (max_id > 0 && loaded.next_commit_id <= max_id) {
        loaded.next_commit_id = (int)max_id + 1;
    }
    if (loaded.next_commit_id <= 0) {
        goto cleanup;
    }

    success = 1;

cleanup:
    if (file != NULL) {
        (void)fclose(file);
    }

    if (!success) {
        free_commit_nodes(loaded.head);
        return 0;
    }

    history->head = loaded.head;
    history->tail = loaded.tail;
    history->total_commits = loaded.total_commits;
    history->next_commit_id = loaded.next_commit_id;
    return 1;
}

void free_commit_history(CommitHistory *history)
{
    if (history == NULL) {
        return;
    }

    free_commit_nodes(history->head);
    free(history);
}

void free_undo_stack(UndoStack *stack)
{
    /* The stack contains non-owning pointers; commit nodes are freed by the history. */
    free(stack);
}
