#include <stdio.h>

int main()
{
    int choice;

    do
    {
        printf("\n===== GITLITE =====\n");
        printf("1. Initialize Repository\n");
        printf("2. Add File\n");
        printf("3. Commit\n");
        printf("4. View History\n");
        printf("5. Create Branch\n");
        printf("6. Search\n");
        printf("7. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                printf("Repository Initialized!\n");
                break;

            case 2:
                printf("File Added!\n");
                break;

            case 3:
                printf("Commit Created!\n");
                break;

            case 4:
                printf("Showing Commit History...\n");
                break;

            case 5:
                printf("Branch Created!\n");
                break;

            case 6:
                printf("Searching...\n");
                break;

            case 7:
                printf("Exiting GitLite...\n");
                break;

            default:
                printf("Invalid Choice!\n");
        }

    } while(choice != 7);

    return 0;
}