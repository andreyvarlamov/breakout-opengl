#include "resource_manager.h"

#include <stdbool.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../util/file_io.h"
#include "texture.h"
#include "shader.h"

//ShaderTable shader_table;
//TextureTable texture_table;

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

Texture2D _load_texture_from_file(const char* file, bool alpha)
{
    Texture2D texture;

    int width, height, nr_channels;
    unsigned char* data = stbi_load(file, &width, &height, &nr_channels, 0);

    // Conversion signed int to unsigned should be safe, as stbi should never
    // return a negative number for image size.
    // This function uses glTextImage2D, which copies data pointed to, so it's
    // safe to free the pointer with stbi_image_free.
    texture2d_generate(
            &texture,
            (unsigned int) width,
            (unsigned int) height,
            alpha,
            data);

    stbi_image_free(data);
    return texture;
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

Texture2D load_texture(
        const char* file,
        bool alpha,
        const char* name)
{
    Texture2D texture2d = _load_texture_from_file(file, alpha);

    // texture_table_add(&texture_table, name, texture2d);

    return texture2d;
}

Texture2D get_texture(const char* name)
{
    //Texture2D texture2d = texture_table_get(&texture_table, name);
    Texture2D texture2d;
    return texture2d;
}

void clear()
{
    //shader_table_clear(&shader_table);
    //texture_table_clear(&texture_table);
}
