#ifndef SEARCH_H
#define SEARCH_H

#include "commit.h"

/*
 * Search for a commit using its commit ID.
 * Returns the matching Commit pointer, or NULL if not found.
 */
Commit* search_commit_by_id(const CommitHistory* history, int commit_id);

/*
 * Search commits whose message contains the given keyword.
 * Prints matching commits.
 * Returns the number of matches found.
 */
int search_commits_by_message(const CommitHistory* history, const char* keyword);

/*
 * Search commits made by a particular author.
 * Prints matching commits.
 * Returns the number of matches found.
 */
int search_commits_by_author(const CommitHistory* history, const char* author);

/*
 * Search commits using a general keyword.
 * The keyword is checked against commit ID, message and author.
 * Returns the number of matching commits.
 */
int search_commits(const CommitHistory* history, const char* keyword);

#endif /* SEARCH_H */