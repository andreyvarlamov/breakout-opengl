#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>

#include <cglm/cglm.h>

#include "gfx.h"

// Sets the current shader as active
void shader_use(GLuint shader_id);

// Compile the shader from given source code
GLuint shader_compile(
        const char* vertex_source,
        const char* fragment_source,
        const char* geometry_source);

// Utility functions
void shader_uniform_mat4( GLuint shader_id, const char* name, mat4 mat );
void shader_uniform_vec2( GLuint shader_id, const char* name, vec2 vec );
void shader_uniform_vec3( GLuint shader_id, const char* name, vec3 vec );
void shader_uniform_vec4( GLuint shader_id, const char* name, vec4 vec );
void shader_uniform_int ( GLuint shader_id, const char* name, int  num );

#endif
