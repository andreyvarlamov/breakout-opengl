#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdbool.h>

#include "gfx.h"

// Generate texture from image data
GLuint texture2d_generate(
        unsigned int width,
        unsigned int height,
        bool alpha,
        unsigned char* data);

// Bind the texture as the current active GL_TEXTURE_2D texture object
void texture2d_bind(GLuint id);

#endif
