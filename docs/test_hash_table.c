
#include "hash_table.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    HashTable ht;
    init_hash_table(&ht);

    Commit c1 = {0}, c2 = {0}, c3 = {0};

    c1.commitID = 10;
    strcpy(c1.message, "First commit");

    c2.commitID = 20;
    strcpy(c2.message, "Second commit");

    c3.commitID = 10 + HASH_TABLE_SIZE;
    strcpy(c3.message, "Collision commit");

    // Test insertion
    assert(hash_insert(&ht, &c1) == 1);
    assert(hash_insert(&ht, &c2) == 1);
    assert(hash_insert(&ht, &c3) == 1);

    // Test searching
    assert(hash_search(&ht, 10) == &c1);
    assert(hash_search(&ht, 20) == &c2);
    assert(hash_search(&ht, 10 + HASH_TABLE_SIZE) == &c3);
    assert(hash_search(&ht, 999) == NULL);

    // Test deletion
    assert(hash_delete(&ht, 10) == 1);
    assert(hash_search(&ht, 10) == NULL);

    // Other collision entry should still exist
    assert(hash_search(&ht, 10 + HASH_TABLE_SIZE) == &c3);

    hash_display(&ht);
    free_hash_table(&ht);

    printf("\nAll Hash Table tests passed!\n");
    return 0;
}
