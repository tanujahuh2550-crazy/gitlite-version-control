
#include <stdio.h>
#include <string.h>
#include "commit.h"
#include "file_manager.h"

#define MAX_PATH_LENGTH 512

static int read_line(const char *prompt, char *buffer, size_t size)
{
    int ch;

    printf("%s", prompt);

    if (fgets(buffer, (int)size, stdin) == NULL)
        return 0;

    size_t len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';
    else
    {
        while ((ch = getchar()) != '\n' && ch != EOF)
        {
            /* Discard input that did not fit in the buffer. */
        }
    }

    return 1;
}

static void clear_pending_files(TrackedFile files[], int *count)
{
    for (int i = 0; i < *count; i++)
    {
        files[i].filename[0] = '\0';
        files[i].content[0] = '\0';
    }

    *count = 0;
}

int main(void)
{
    int choice = 0;
    int pending_count = 0;

    TrackedFile pending_files[MAX_FILES] = {0};

    CommitHistory *history = init_commit_history();
    UndoStack *undo_stack = init_undo_stack();

    if (history == NULL || undo_stack == NULL)
    {
        printf("Error: Could not initialize Commit Engine.\n");
        free_commit_history(history);
        free_undo_stack(undo_stack);
        return 1;
    }

    do
    {
        printf("\n===== GITLITE =====\n");
        printf("1. Initialize Repository (coming next)\n");
        printf("2. Add File from Disk\n");
        printf("3. Create Commit\n");
        printf("4. View Commit History\n");
        printf("5. Create Branch (coming next)\n");
        printf("6. Search (coming next)\n");
        printf("7. Exit\n");
        printf("8. Undo Latest Commit\n");
        printf("9. Save Commit History\n");
        printf("10. Load Commit History\n");

        char input[32];

        if (!read_line("Enter your choice: ", input, sizeof(input)))
            break;

        if (sscanf(input, "%d", &choice) != 1)
        {
            printf("Invalid choice. Enter a number.\n");
            continue;
        }

        switch (choice)
        {
            case 1:
                printf("Repository initialization will be connected next.\n");
                break;

            case 2:
            {
                if (pending_count >= MAX_FILES)
                {
                    printf("File limit reached (%d files).\n", MAX_FILES);
                    break;
                }

                char path[MAX_PATH_LENGTH];
                char filename[MAX_FILENAME_LENGTH];
                char content[MAX_FILE_CONTENT_LENGTH];

                if (!read_line("Enter the full file path: ",
                               path, sizeof(path)))
                    break;

                if (path[0] == '\0')
                {
                    printf("Path cannot be empty.\n");
                    break;
                }

                if (!read_file_content(path, content, sizeof(content)))
                {
                    printf("Could not add file. Check the path and file size.\n");
                    break;
                }

                /*
                 * Store the path as the tracked filename.
                 * A shorter filename can be used if needed.
                 */
                size_t path_len = strlen(path);

                if (path_len >= sizeof(filename))
                {
                    printf("Path is too long to track.\n");
                    break;
                }

                strcpy(filename, path);

                int duplicate = 0;

                for (int i = 0; i < pending_count; i++)
                {
                    if (strcmp(pending_files[i].filename, filename) == 0)
                    {
                        duplicate = 1;
                        break;
                    }
                }

                if (duplicate)
                {
                    printf("This file is already added.\n");
                    break;
                }

                strcpy(pending_files[pending_count].filename, filename);
                strcpy(pending_files[pending_count].content, content);
                pending_count++;

                printf("File added successfully from disk!\n");
                printf("Pending files: %d/%d\n", pending_count, MAX_FILES);
                break;
            }

            case 3:
            {
                if (pending_count == 0)
                {
                    printf("No files added. Choose option 2 first.\n");
                    break;
                }

                char message[MAX_COMMIT_MESSAGE_LENGTH];
                char author[MAX_AUTHOR_LENGTH];

                if (!read_line("Enter commit message: ",
                               message, sizeof(message)) ||
                    !read_line("Enter author name: ",
                               author, sizeof(author)))
                    break;

                if (message[0] == '\0' || author[0] == '\0')
                {
                    printf("Message and author cannot be empty.\n");
                    break;
                }

                if (make_commit(history, undo_stack, message, author,
                                pending_files, pending_count))
                {
                    printf("Commit created successfully!\n");
                    clear_pending_files(pending_files, &pending_count);
                }
                else
                {
                    printf("Could not create commit. Pending files retained.\n");
                }

                break;
            }

            case 4:
                display_history(history);
                break;

            case 5:
                printf("Branch management will be connected next.\n");
                break;

            case 6:
                printf("Search will be connected next.\n");
                break;

            case 7:
                printf("Exiting GitLite...\n");
                break;

            case 8:
                if (rollback_commit(history, undo_stack))
                    printf("Latest commit undone.\n");
                else
                    printf("No commit available to undo.\n");
                break;

            case 9:
                if (save_history_to_file(history, "gitlite_history.txt"))
                    printf("Commit history saved.\n");
                else
                    printf("Could not save commit history.\n");
                break;

            case 10:
                printf("History loading will be checked separately before integration.\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 7);

    free_commit_history(history);
    free_undo_stack(undo_stack);

    return 0;
}
