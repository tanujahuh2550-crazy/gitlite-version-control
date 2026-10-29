
#include <stdio.h>
#include <string.h>
#include "file_manager.h"

int save_data(const char *filename, const char *data)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        perror("Error opening file for writing");
        return 0;
    }

    if (fputs(data, file) == EOF)
    {
        perror("Error writing file");
        fclose(file);
        return 0;
    }

    if (fclose(file) != 0)
    {
        perror("Error closing file");
        return 0;
    }

    printf("Data saved successfully.\n");
    return 1;
}

int load_data(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("Error opening file for reading");
        return 0;
    }

    int ch;

    while ((ch = fgetc(file)) != EOF)
        putchar(ch);

    if (ferror(file))
    {
        perror("Error reading file");
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

int read_file_content(const char *path, char *buffer, size_t buffer_size)
{
    if (path == NULL || buffer == NULL || buffer_size == 0)
        return 0;

    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        perror("Could not open source file");
        return 0;
    }

    size_t bytes_read = fread(buffer, 1, buffer_size - 1, file);

    if (ferror(file))
    {
        perror("Could not read source file");
        fclose(file);
        return 0;
    }

    if (bytes_read == buffer_size - 1)
    {
        int extra = fgetc(file);

        if (extra != EOF)
        {
            printf("Error: File exceeds the supported content size.\n");
            fclose(file);
            return 0;
        }

        if (ferror(file))
        {
            perror("Could not read source file");
            fclose(file);
            return 0;
        }
    }

    buffer[bytes_read] = '\0';

    if (fclose(file) != 0)
    {
        perror("Could not close source file");
        return 0;
    }

    return 1;
}
