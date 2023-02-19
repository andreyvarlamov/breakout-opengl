#include "file_io.h"

void read_file(const char* path, char* content)
{
    FILE* f;

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
}
