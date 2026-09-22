#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Hash function
 * Converts a commit ID into an index in the hash table.
 */
int hash_function(int key)
{
    if (key < 0)
        key = -key;

    return key % HASH_TABLE_SIZE;
}

/*
 * Initialize all hash table buckets.
 */
void init_hash_table(HashTable* ht)
{
    if (ht == NULL)
        return;

    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        ht->table[i] = NULL;
    }
}

/*
 * Insert a commit into the hash table.
 * Separate chaining is used to handle collisions.
 */
int hash_insert(HashTable* ht, Commit* commit)
{
    if (ht == NULL || commit == NULL)
        return 0;

    int index = hash_function(commit->commitID);

    HashNode* current = ht->table[index];

    // Check whether this commit ID already exists.
    while (current != NULL)
    {
        if (current->key == commit->commitID)
        {
            current->commit = commit;
            return 1;
        }

        current = current->next;
    }

    // Create a new node.
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));

    if (newNode == NULL)
        return 0;

    newNode->key = commit->commitID;
    newNode->commit = commit;

    // Insert at the beginning of the chain.
    newNode->next = ht->table[index];
    ht->table[index] = newNode;

    return 1;
}

/*
 * Search for a commit using its commit ID.
 */
Commit* hash_search(HashTable* ht, int commitID)
{
    if (ht == NULL)
        return NULL;

    int index = hash_function(commitID);

    HashNode* current = ht->table[index];

    while (current != NULL)
    {
        if (current->key == commitID)
        {
            return current->commit;
        }

        current = current->next;
    }

    return NULL;
}

/*
 * Delete a commit from the hash table.
 *
 * Note:
 * This only removes the hash table node.
 * It does not free the Commit itself because the Commit
 * may still belong to CommitHistory.
 */
int hash_delete(HashTable* ht, int commitID)
{
    if (ht == NULL)
        return 0;

    int index = hash_function(commitID);

    HashNode* current = ht->table[index];
    HashNode* previous = NULL;

    while (current != NULL)
    {
        if (current->key == commitID)
        {
            if (previous == NULL)
            {
                ht->table[index] = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            free(current);
            return 1;
        }

        previous = current;
        current = current->next;
    }

    return 0;
}

/*
 * Display all hash table entries.
 */
void hash_display(const HashTable* ht)
{
    if (ht == NULL)
        return;

    printf("\n========== HASH TABLE ==========\n");

    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        HashNode* current = ht->table[i];

        if (current != NULL)
        {
            printf("[%d] ", i);

            while (current != NULL)
            {
                printf("Commit ID: %d", current->key);

                if (current->commit != NULL)
                {
                    printf(" | Message: %s",
                           current->commit->message);
                }

                if (current->next != NULL)
                {
                    printf(" -> ");
                }

                current = current->next;
            }

            printf("\n");
        }
    }

    printf("================================\n");
}

/*
 * Free all hash table nodes.
 *
 * The Commit objects themselves are not freed here because
 * they are owned by the commit history.
 */
void free_hash_table(HashTable* ht)
{
    if (ht == NULL)
        return;

    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        HashNode* current = ht->table[i];

        while (current != NULL)
        {
            HashNode* temp = current;
            current = current->next;
            free(temp);
        }

        ht->table[i] = NULL;
    }
}
