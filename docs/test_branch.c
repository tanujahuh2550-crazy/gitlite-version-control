
#include "branch.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    root = malloc(sizeof(BranchNode));
    assert(root != NULL);

    strcpy(root->branchName, "main");
    root->commitCount = 0;
    root->parent = NULL;
    root->firstChild = NULL;
    root->nextSibling = NULL;

    assert(findBranch("main") == root);
    assert(createBranch("feature-login", "main") == 1);
    assert(findBranch("feature-login") != NULL);
    assert(findBranch("feature-login")->parent == root);

    assert(createBranch("feature-search", "main") == 1);
    assert(findBranch("feature-search") != NULL);

    assert(findBranch("missing") == NULL);

    puts("All Branch tests passed.");

    free(root->firstChild->nextSibling);
    free(root->firstChild);
    free(root);
    root = NULL;

    return 0;
}
