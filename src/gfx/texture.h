#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdbool.h>

typedef struct Texture2D
{
    unsigned int id;
    // Texture image dimensions
    unsigned int width;
    unsigned int height;
    // Texture format
    unsigned int internal_format; //format of texture object
    unsigned int image_format; // format of loaded image
    // Texture configuration
    unsigned int wrap_s; // wrapping mode on S axis
    unsigned int wrap_t; // wrapping mode on T axis
    unsigned int filter_min; // filter mode if texture pixels < screen pixels
    unsigned int filter_mag; // filter mode if texture pixels > screen pixels
} Texture2D;

Texture2D* texture2d_create();
void texture2d_destroy(Texture2D** texture2d);

// Generate texture from image data
void texture2d_generate(
        Texture2D* texture2d,
        unsigned int width,
        unsigned int height,
        bool alpha,
        unsigned char* data);

// Bind the texture as the current active GL_TEXTURE_2D texture object
void texture2d_bind(
        const Texture2D* texture2d);

#endif
