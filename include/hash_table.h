#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "commit.h"

#define HASH_TABLE_SIZE 101

// Node of the hash table
typedef struct HashNode {
    int key;
    Commit* commit;
    struct HashNode* next;
} HashNode;

// Hash table structure
typedef struct {
    HashNode* table[HASH_TABLE_SIZE];
} HashTable;

// Initialize the hash table
void init_hash_table(HashTable* ht);

// Hash function
int hash_function(int key);

// Insert a commit into the hash table
int hash_insert(HashTable* ht, Commit* commit);

// Search for a commit using its commit ID
Commit* hash_search(HashTable* ht, int commitID);

// Delete a commit using its commit ID
int hash_delete(HashTable* ht, int commitID);

// Display all entries in the hash table
void hash_display(const HashTable* ht);

// Free the hash table nodes
void free_hash_table(HashTable* ht);

#endif
