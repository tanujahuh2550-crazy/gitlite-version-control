#include <stdio.h>
#include "../include/file_manager.h"

int main(void)
{
    const char *filename = "test_data.txt";
    const char *data = "GitLite File Handling Test";

    printf("Saving data...\n");
    save_data(filename, data);

    printf("\nLoading data...\n");
    load_data(filename);

    printf("\n\nTest completed.\n");
    return 0;
}
