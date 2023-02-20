#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <stdbool.h>

#include "gfx.h"
#include "tex_type.h"
#include "texture.h"
#include "shader_type.h"
#include "shader.h"

// Load and generate a shader program from file loading vertex, fragment (and
// geometry) shader's source code.
GLuint load_shader(
        const char* vShaderFile,
        const char* fShaderFile,
        const char* gShaderFile,
        ShaderType shader_type);

// Retrieve a stored shader
GLuint get_shader(ShaderType shader_type);

// Load and generate a texture from file
GLuint load_texture(
        const char* file,
        bool alpha,
        TexType tex_type);

// Retrieve a stored texture
GLuint get_texture(TexType tex_type);

// Properly deallocate all loaded resources
void clear();

#endif
