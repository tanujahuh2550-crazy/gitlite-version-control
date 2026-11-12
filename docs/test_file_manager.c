
#include "file_manager.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    const char *filename = "test_data.txt";
    const char *original = "Hello GitLite! File handling test.";

    char buffer[100] = {0};

    // Test 1: Save data
    assert(save_data(filename, original) == 1);
    printf("Save test passed!\n");

    // Test 2: Read file content
    assert(read_file_content(filename, buffer, sizeof(buffer)) == 1);
    assert(strcmp(buffer, original) == 0);
    printf("Read content test passed!\n");

    // Test 3: Load and display data
    printf("\nLoaded data:\n");
    assert(load_data(filename) == 1);
    printf("\nLoad test passed!\n");

    // Test 4: Invalid file
    assert(read_file_content("nonexistent.txt", buffer, sizeof(buffer)) == 0);

    // Test 5: Invalid buffer
    assert(read_file_content(filename, buffer, 0) == 0);

    printf("\nAll File Manager tests passed!\n");

    remove(filename);
    return 0;
}
