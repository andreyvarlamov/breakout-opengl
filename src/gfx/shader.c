#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gfx.h"

void use(GLuint shader_id)
{
}

void _check_compile_errors(unsigned int object, const char* type)
{
    int success;
    char infoLog[1024];

    if (strcmp(type, "PROGRAM") != 0)
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            printf("| ERROR::SHADER: Compile-time error: Type: %s\n%s\n"
                   "-- -------------------------------------- --\n",
                   type, infoLog);
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            printf("| ERROR::SHADER: Link-time error: Type: %s\n%s\n"
                   "-- -------------------------------------- --\n",
                   type, infoLog);
        }
    }
}

GLuint shader_compile(
        const char* vertex_source,
        const char* fragment_source,
        const char* geometry_source)
{
    unsigned int s_vertex, s_fragment, s_geometry;
    GLuint shader_id;

    s_vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(s_vertex, 1, &vertex_source, NULL);
    glCompileShader(s_vertex);
    _check_compile_errors(s_vertex, "VERTEX");

    s_fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(s_fragment, 1, &fragment_source, NULL);
    glCompileShader(s_fragment);
    _check_compile_errors(s_fragment, "FRAGMENT");

    if (geometry_source)
    {
        s_geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(s_geometry, 1, &geometry_source, NULL);
        glCompileShader(s_geometry);
        _check_compile_errors(s_geometry, "GEOMETRY");
    }

    shader_id = glCreateProgram();
    glAttachShader(shader_id, s_vertex);
    glAttachShader(shader_id, s_fragment);
    if (geometry_source)
    {
        glAttachShader(shader_id, s_geometry);
    }
    glLinkProgram(shader_id);
    _check_compile_errors(shader_id, "PROGRAM");

    glDeleteShader(s_vertex);
    glDeleteShader(s_fragment);
    if (geometry_source)
    {
        glDeleteShader(s_geometry);
    }

    return shader_id;
}

void shader_set_float(
        GLuint shader_id,
        const char* name,
        float value,
        bool useShader)
{
}

void shader_set_int(
        GLuint shader_id,
        const char* name,
        int value,
        bool useShader)
{
}

void shader_set_vector2f(
        GLuint shader_id,
        const char* name,
        float x,
        float y,
        bool useShader)
{
}

void shader_set_vector3f(
        GLuint shader_id,
        const char* name,
        float x,
        float y,
        float z,
        bool useShader)
{
}

void shader_set_vector4f(
        GLuint shader_id,
        const char* name,
        float x,
        float y,
        float z,
        float w,
        bool useShader)
{
}
