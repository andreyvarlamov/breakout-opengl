#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>

typedef struct Shader
{
    unsigned int id;
} Shader;

Shader* shader_create();
void shader_destroy(Shader** shader);

// Sets the current shader as active
Shader* use(Shader* shader);

// Compile the shader from given source code
void shader_compile(
        Shader* shader,
        const char* vertex_source,
        const char* fragment_source,
        const char* geometry_source);

// Utility functions
void shader_set_float(
        Shader* shader,
        const char* name,
        float value,
        bool useShader);

void shader_set_int(
        Shader* shader,
        const char* name,
        int value,
        bool useShader);

void shader_set_vector2f(
        Shader* shader,
        const char* name,
        float x,
        float y,
        bool useShader);

void shader_set_vector3f(
        Shader* shader,
        const char* name,
        float x,
        float y,
        float z,
        bool useShader);

void shader_set_vector4f(
        Shader* shader,
        const char* name,
        float x,
        float y,
        float z,
        float w,
        bool useShader);

#endif
