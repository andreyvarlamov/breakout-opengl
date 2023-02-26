#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <stdbool.h>

#include "gfx.h"
#include "tex_type.h"
#include "texture.h"
#include "shader_type.h"
#include "shader.h"

// Read all shaders and load them into OpenGL
void rm_shader_load_all();

// Read all textures and load them into OpenGL
void rm_tex_load_all();

// Get OpenGL shader ID by type of shader
GLuint rm_shader_get( ShaderType shader_type );

// Get OpenGL texture ID by type of texture
GLuint rm_tex_get( TexType tex_type );

#endif
