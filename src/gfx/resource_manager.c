#include "resource_manager.h"

#include <stdbool.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../util/file_io.h"
#include "gfx.h"
#include "tex_type.h"
#include "texture.h"
#include "shader.h"

GLuint _tex_table[TEX_TYPE_COUNT];

Shader _load_shader_from_file(
        const char* vShaderFile,
        const char* fShaderFile,
        const char* gShaderFile)
{
    char* vShaderCode = read_file(vShaderFile);
    char* fShaderCode = read_file(fShaderFile);
    char* gShaderCode = NULL;

    if (gShaderFile)
    {
        gShaderCode = read_file(gShaderFile);
    }

    Shader shader;
    shader_compile(&shader, vShaderCode, fShaderCode, gShaderCode);

    free(vShaderCode);
    vShaderCode = NULL;

    free(fShaderCode);
    fShaderCode = NULL;

    free(gShaderCode);
    gShaderCode = NULL;

    return shader;
}

Shader load_shader(
        const char* vShaderFile,
        const char* fShaderFile,
        const char* gShaderFile,
        const char* name)
{
    Shader shader = _load_shader_from_file(
            vShaderFile,
            fShaderFile,
            gShaderFile);

    // shader_table_add(&shader_table, name, shader);

    return shader;
}

Shader get_shader(const char* name)
{
    //Shader shader = shader_table_get(&shader_table, name);
    Shader shader;
    return shader;
}

GLuint _load_texture_from_file(const char* file, bool alpha)
{
    int width, height, nr_channels;
    unsigned char* data = stbi_load(file, &width, &height, &nr_channels, 0);

    // Conversion signed int to unsigned should be safe, as stbi should never
    // return a negative number for image size.
    // This function uses glTextImage2D, which copies data pointed to, so it's
    // safe to free the pointer with stbi_image_free.
    GLuint id = texture2d_generate(
            (unsigned int) width,
            (unsigned int) height,
            alpha,
            data);

    stbi_image_free(data);
    return id;
}

void load_texture(
        const char* file,
        bool alpha,
        TexType tex_type)
{
    _tex_table[tex_type] = _load_texture_from_file(file, alpha);
}

GLuint get_texture(TexType tex_type)
{
    return _tex_table[tex_type];
}

void clear()
{
    //shader_table_clear(&shader_table);
    //texture_table_clear(&texture_table);
}
