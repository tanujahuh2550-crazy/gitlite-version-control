#include <stdio.h>
#include "../include/file_manager.h"

void save_data(const char *filename, const char *data)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    fprintf(file, "%s", data);

    fclose(file);

    printf("Data saved successfully.\n");
}

void load_data(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("Error: Could not open file for reading.\n");
        return;
    }

    char ch;

    while ((ch = fgetc(file)) != EOF)
    {
        putchar(ch);
    }

    fclose(file);
}