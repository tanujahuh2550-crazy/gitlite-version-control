#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stddef.h>

int save_data(const char *filename, const char *data);
int load_data(const char *filename);
int read_file_content(const char *path, char *buffer, size_t buffer_size);

#endif
