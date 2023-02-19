#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <stdbool.h>

#include "gfx.h"
#include "tex_type.h"
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
void load_texture(
        const char* file,
        bool alpha,
        TexType tex_type);

// Retrieve a stored texture
GLuint get_texture(TexType tex_type);

// Properly deallocate all loaded resources
void clear();

#endif
