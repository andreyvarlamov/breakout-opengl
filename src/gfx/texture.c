#include "texture.h"

#include <stdlib.h>

Texture2D* texture2d_create()
{
    Texture2D* texture2d = (Texture2D*) malloc(sizeof(Texture2D));
    return texture2d;
}

void texture2d_destroy(Texture2D** texture2d)
{
    free(*texture2d);
    *texture2d = NULL;
}

void texture2d_generate(
        Texture2D* texture2d,
        unsigned int width,
        unsigned int height,
        unsigned char* data)
{
}

void texture2d_bind(
        Texture2D* texture2d)
{
}
