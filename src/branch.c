#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/branch.h"

// Root of the branch tree
BranchNode *root = NULL;

// Search for a branch by name
BranchNode *findBranch(const char *name) {
    if (root == NULL || name == NULL)
        return NULL;

    BranchNode *current = root;

    // Search the tree using DFS
    BranchNode *stack[1000];
    int top = 0;

    stack[top++] = current;

    while (top > 0) {
        current = stack[--top];

        if (strcmp(current->branchName, name) == 0)
            return current;

        BranchNode *child = current->firstChild;

        while (child != NULL) {
            if (top < 1000)
                stack[top++] = child;

            child = child->nextSibling;
        }
    }

    return NULL;
}

// Create a new branch under a parent branch
int createBranch(const char *name, const char *parentName) {
    if (name == NULL || parentName == NULL ||
        strlen(name) == 0 || strlen(name) >= 50) {
        printf("Invalid branch name!\n");
        return 0;
    }

    // Check whether the branch already exists
    if (findBranch(name) != NULL) {
        printf("Branch already exists!\n");
        return 0;
    }

    // Find the parent branch
    BranchNode *parent = findBranch(parentName);

    if (parent == NULL) {
        printf("Parent branch not found!\n");
        return 0;
    }

    // Allocate memory for the new branch
    BranchNode *newBranch =
        (BranchNode *)malloc(sizeof(BranchNode));

    if (newBranch == NULL) {
        printf("Memory allocation failed!\n");
        return 0;
    }

    // Initialize the new branch
    strcpy(newBranch->branchName, name);
    newBranch->commitCount = 0;
    newBranch->parent = parent;
    newBranch->firstChild = NULL;
    newBranch->nextSibling = NULL;

    // Insert as the first child of the parent
    newBranch->nextSibling = parent->firstChild;
    parent->firstChild = newBranch;

    printf("Branch '%s' created successfully!\n", name);
    return 1;
}