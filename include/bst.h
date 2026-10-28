#ifndef BST_H
#define BST_H

#include "commit.h"

// Binary Search Tree node
typedef struct BSTNode {
    int key;
    Commit* commit;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

// Create a new BST node
BSTNode* bst_create_node(Commit* commit);

// Insert a commit into the BST using commitID as the key
BSTNode* bst_insert(BSTNode* root, Commit* commit);

// Search for a commit by commitID
Commit* bst_search(BSTNode* root, int commitID);

// Display BST in inorder traversal
void bst_inorder(BSTNode* root);

// Free all BST nodes
void bst_free(BSTNode* root);

#endif