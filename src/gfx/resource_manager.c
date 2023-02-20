#include "resource_manager.h"

#include <stdbool.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../util/file_io.h"
#include "gfx.h"
#include "tex_type.h"
#include "texture.h"
#include "shader_type.h"
#include "shader.h"

GLuint _tex_table[TEX_TYPE_COUNT];
GLuint _shader_table[SHADER_TYPE_COUNT];

GLuint _load_shader_from_file(
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

    GLuint id = shader_compile(vShaderCode, fShaderCode, gShaderCode);

    free(vShaderCode);
    vShaderCode = NULL;

    free(fShaderCode);
    fShaderCode = NULL;

    free(gShaderCode);
    gShaderCode = NULL;

    return id;
}

GLuint load_shader(
        const char* vShaderFile,
        const char* fShaderFile,
        const char* gShaderFile,
        ShaderType shader_type)
{
    GLuint id = _load_shader_from_file(vShaderFile, fShaderFile, gShaderFile);
    _shader_table[shader_type] = id;
    return id;
}

GLuint get_shader(ShaderType shader_type)
{
    return _shader_table[shader_type];
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

GLuint load_texture(
        const char* file,
        bool alpha,
        TexType tex_type)
{
    GLuint id = _load_texture_from_file(file, alpha);
    _tex_table[tex_type] = id;
    return id;
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
