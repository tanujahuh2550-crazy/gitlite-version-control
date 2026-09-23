#ifndef SORTING_H
#define SORTING_H

#include "commit.h"

/* Sort commit history using Merge Sort */
void merge_sort_commits(CommitHistory* history);

/* Sort commit history using Quick Sort */
void quick_sort_commits(CommitHistory* history);

/* Display the sorted commit history */
void display_sorted_commits(const CommitHistory* history);

#endif