#include "../include/bst.h"
#include <stdio.h>
#include <stdlib.h>

// Create a new BST node
BSTNode* bst_create_node(Commit* commit) {
    if (commit == NULL) {
        return NULL;
    }

    BSTNode* new_node = (BSTNode*)malloc(sizeof(BSTNode));

    if (new_node == NULL) {
        return NULL;
    }

    new_node->key = commit->commitID;
    new_node->commit = commit;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

// Insert a commit into the BST
BSTNode* bst_insert(BSTNode* root, Commit* commit) {
    if (commit == NULL) {
        return root;
    }

    // Empty tree
    if (root == NULL) {
        return bst_create_node(commit);
    }

    // Insert into left subtree
    if (commit->commitID < root->key) {
        root->left = bst_insert(root->left, commit);
    }

    // Insert into right subtree
    else if (commit->commitID > root->key) {
        root->right = bst_insert(root->right, commit);
    }

    // Duplicate commitID - do nothing
    else {
        return root;
    }

    return root;
}

// Search for a commit by commitID
Commit* bst_search(BSTNode* root, int commitID) {
    if (root == NULL) {
        return NULL;
    }

    if (commitID == root->key) {
        return root->commit;
    }

    if (commitID < root->key) {
        return bst_search(root->left, commitID);
    }

    return bst_search(root->right, commitID);
}

// Inorder traversal
void bst_inorder(BSTNode* root) {
    if (root == NULL) {
        return;
    }

    bst_inorder(root->left);

    if (root->commit != NULL) {
        printf("Commit ID: %d | Message: %s\n",
               root->commit->commitID,
               root->commit->message);
    }

    bst_inorder(root->right);
}

// Free BST nodes
void bst_free(BSTNode* root) {
    if (root == NULL) {
        return;
    }

    bst_free(root->left);
    bst_free(root->right);

    free(root);
}