#include "shader.h"

#include <stdlib.h>

Shader* shader_create()
{
    Shader* shader = (Shader*) malloc(sizeof(Shader));
    return shader;
}

void shader_destroy(Shader** shader)
{
    free(*shader);
    *shader = NULL;
}

Shader* use(Shader* shader)
{
    return shader;
}

void shader_compile(
        Shader* shader,
        const char* vertex_source,
        const char* fragment_source,
        const char* geometry_source)
{
}

void shader_set_float(
        Shader* shader,
        const char* name,
        float value,
        bool useShader)
{
}

void shader_set_int(
        Shader* shader,
        const char* name,
        int value,
        bool useShader)
{
}

void shader_set_vector2f(
        Shader* shader,
        const char* name,
        float x,
        float y,
        bool useShader)
{
}

void shader_set_vector3f(
        Shader* shader,
        const char* name,
        float x,
        float y,
        float z,
        bool useShader)
{
}

void shader_set_vector4f(
        Shader* shader,
        const char* name,
        float x,
        float y,
        float z,
        float w,
        bool useShader)
{
}
