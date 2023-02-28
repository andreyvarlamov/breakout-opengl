#include "texture.h"

#include <stdbool.h>
#include <stdlib.h>

#include "gfx.h"

GLuint texture2d_generate(
        unsigned int width,
        unsigned int height,
        bool alpha,
        unsigned char* data)
{
    GLuint id = 0;
    unsigned int internal_format;
    unsigned int image_format;
    unsigned int wrap_s;
    unsigned int wrap_t;
    unsigned int filter_min;
    unsigned int filter_mag;

    // Set gl id for the texture
    glGenTextures(1, &id);

    if (alpha)
    {
        internal_format = GL_RGBA;
        image_format = GL_RGBA;
    }
    else
    {
        internal_format = GL_RGB;
        image_format = GL_RGB;
    }

    wrap_s = GL_REPEAT;
    wrap_t = GL_REPEAT;
    // wrap_s = GL_CLAMP_TO_BORDER;
    // wrap_t = GL_CLAMP_TO_BORDER;
    filter_min = GL_LINEAR;
    filter_mag = GL_LINEAR;

    // Create texture
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            internal_format,
            width,
            height,
            0,
            image_format,
            GL_UNSIGNED_BYTE,
            data);

    // Set texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return id;
}

void texture2d_bind(GLuint id)
{
    glBindTexture(GL_TEXTURE_2D, id);
}
