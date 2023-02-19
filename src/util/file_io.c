#include "file_io.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_file(const char* path)
{
    FILE* f;
    long len;
    char* content;

    f = fopen(path, "rb");
    if (f == NULL)
    {
        printf("Error loading file at %s\n", path);
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    len = ftell(f);
    assert(len > 0);
    fseek(f, 0, SEEK_SET);
    content = calloc(1, len);
    assert(content != NULL);
    fread(content, 1, len, f);
    assert(strlen(content) > 0);
    fclose(f);

    return content;
}
