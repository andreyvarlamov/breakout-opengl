#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <stdbool.h>

#include "texture.h"
#include "shader.h"

//extern ShaderTable shader_table;
//extern TextureTable texture_table;

// Load and generate a shader program from file loading vertex, fragment (and
// geometry) shader's source code.
Shader load_shader(
        const char* vShaderFile,
        const char* fShaderFile,
        const char* gShaderFile,
        const char* name);

// Retrieve a stored shader
Shader get_shader(const char* name);

// Load and generate a texture from file
Texture2D load_texture(
        const char* file,
        bool alpha,
        const char* name);

// Retrieve a stored texture
Texture2D get_texture(const char* name);

// Properly deallocate all loaded resources
void clear();

#endif
