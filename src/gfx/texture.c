#include "texture.h"

#include <stdbool.h>
#include <stdlib.h>

#include "gfx.h"

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
        Texture2D* tex,
        unsigned int width,
        unsigned int height,
        bool alpha,
        unsigned char* data)
{
    // Set gl id for the texture
    glGenTextures(1, &(tex->id));

    tex->width = width;
    tex->height = height;

    if (alpha)
    {
        tex->internal_format = GL_RGBA;
        tex->image_format = GL_RGBA;
    }
    else
    {
        tex->internal_format = GL_RGB;
        tex->image_format = GL_RGB;
    }

    tex->wrap_s = GL_REPEAT;
    tex->wrap_t = GL_REPEAT;
    tex->filter_min = GL_LINEAR;
    tex->filter_mag = GL_LINEAR;

    // Create texture
    glBindTexture(GL_TEXTURE_2D, tex->id);
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            tex->internal_format,
            tex->width,
            tex->height,
            0,
            tex->image_format,
            GL_UNSIGNED_BYTE,
            data);

    // Set texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tex->wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tex->wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex->filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tex->filter_mag);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture2d_bind(
        const Texture2D* tex)
{
    glBindTexture(GL_TEXTURE_2D, tex->id);
}
