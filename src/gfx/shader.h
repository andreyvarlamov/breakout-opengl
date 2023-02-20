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
void shader_uniform_mat4(GLuint shader_id, const char* name, mat4 mat);
void shader_uniform_vec3(GLuint shader_id, const char* name, vec3 vec);

void shader_set_float(
        GLuint shader_id,
        const char* name,
        float value,
        bool useShader);

void shader_set_int(
        GLuint shader_id,
        const char* name,
        int value,
        bool useShader);

void shader_set_vector2f(
        GLuint shader_id,
        const char* name,
        float x,
        float y,
        bool useShader);

void shader_set_vector3f(
        GLuint shader_id,
        const char* name,
        float x,
        float y,
        float z,
        bool useShader);

void shader_set_vector4f(
        GLuint shader_id,
        const char* name,
        float x,
        float y,
        float z,
        float w,
        bool useShader);

#endif
