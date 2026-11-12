#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "commit.h"

int initialize_repository(const char *path);
int repository_exists(const char *path);

#endif