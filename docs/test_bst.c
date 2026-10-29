
#include "bst.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    Commit c1 = {0};
    Commit c2 = {0};
    Commit c3 = {0};

    c1.commitID = 20;
    snprintf(c1.message, sizeof(c1.message), "First commit");

    c2.commitID = 10;
    snprintf(c2.message, sizeof(c2.message), "Second commit");

    c3.commitID = 30;
    snprintf(c3.message, sizeof(c3.message), "Third commit");

    BSTNode *root = NULL;

    root = bst_insert(root, &c1);
    root = bst_insert(root, &c2);
    root = bst_insert(root, &c3);

    assert(root != NULL);
    assert(bst_search(root, 20) == &c1);
    assert(bst_search(root, 10) == &c2);
    assert(bst_search(root, 30) == &c3);
    assert(bst_search(root, 99) == NULL);

    printf("Inorder traversal:\n");
    bst_inorder(root);

    bst_free(root);

    puts("All BST tests passed.");
    return 0;
}
