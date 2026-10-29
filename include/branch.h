
#ifndef BRANCH_H
#define BRANCH_H

// Structure representing a GitLite branch
typedef struct BranchNode {
    char branchName[50];

    int commitCount;

    struct BranchNode *parent;
    struct BranchNode *firstChild;
    struct BranchNode *nextSibling;

} BranchNode;

// Root of the branch tree
extern BranchNode *root;

// Function declarations
BranchNode *findBranch(const char *name);

int createBranch(const char *name, const char *parentName);

#endif

