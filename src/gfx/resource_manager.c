#include "resource_manager.h"

#include <stdbool.h>
#include <stdlib.h>

#include "../util/file_io.h"
#include "texture.h"
#include "shader.h"

//ShaderTable shader_table;
//TextureTable texture_table;

Shader _loadShaderFromFile(
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
    Shader shader = _loadShaderFromFile(
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
    //Texture2D texture2d = _loadTextureFromFile(file, alpha);
    Texture2D texture2d;

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
