
#include "repository.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    const char *path = "test_gitlite_repo";

    // Test 1: Repository should not exist initially
    assert(repository_exists(path) == 0);
    printf("Non-existing repository test passed!\n");

    // Test 2: Initialize repository
    assert(initialize_repository(path) == 1);
    printf("Repository initialization test passed!\n");

    // Test 3: Check whether repository exists
    assert(repository_exists(path) == 1);
    printf("Repository existence test passed!\n");

    // Test 4: Initialize an already existing directory
    assert(initialize_repository(path) == 1);
    printf("Existing directory test passed!\n");

    // Test 5: Invalid paths
    assert(initialize_repository(NULL) == 0);
    assert(initialize_repository("") == 0);
    assert(repository_exists(NULL) == 0);
    assert(repository_exists("") == 0);
    printf("Invalid path tests passed!\n");

    // Cleanup
    remove(path);

    printf("\nAll Repository tests passed!\n");
    return 0;
}
