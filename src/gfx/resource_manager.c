#include "resource_manager.h"

#include <stdbool.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../util/file_io.h"
#include "gfx.h"
#include "tex_type.h"
#include "texture.h"
#include "shader_type.h"
#include "shader.h"

GLuint _tex_table[TEX_COUNT];
GLuint _shader_table[SHADER_COUNT];

static void _shader_load(
    const char* vShaderFile,
    const char* fShaderFile,
    const char* gShaderFile,
    ShaderType shader_type
)
{
    char* vShaderCode = read_file( vShaderFile );
    char* fShaderCode = read_file( fShaderFile );
    char* gShaderCode = NULL;

    if ( gShaderFile )
    {
        gShaderCode = read_file( gShaderFile );
    }

    GLuint id = shader_compile( vShaderCode, fShaderCode, gShaderCode );

    free( vShaderCode );
    vShaderCode = NULL;

    free( fShaderCode );
    fShaderCode = NULL;

    free( gShaderCode );
    gShaderCode = NULL;

    _shader_table[shader_type] = id;
}

static void _tex_load(
    const char* file,
    bool alpha,
    TexType tex_type
)
{
    int width, height, nr_channels;
    unsigned char* data = stbi_load( file, &width, &height, &nr_channels, 0 );

    // Conversion signed int to unsigned should be safe, as stbi should never
    // return a negative number for image size.
    // This function uses glTextImage2D, which copies data pointed to, so it's
    // safe to free the pointer with stbi_image_free.
    GLuint id = texture2d_generate(
        ( unsigned int ) width,
        ( unsigned int ) height,
        alpha,
        data
    );

    stbi_image_free( data );

    _tex_table[tex_type] = id;
}

void rm_shader_load_all()
{
    _shader_load(
        "res/shaders/quad.vs",
        "res/shaders/quad.fs",
        NULL,
        SHADER_QUAD
    );

    _shader_load(
        "res/shaders/particle.vs",
        "res/shaders/particle.fs",
        NULL,
        SHADER_PARTICLE
    );
}

void rm_tex_load_all()
{
    _tex_load( "res/textures/awesomeface.png", true, TEX_FACE );
    _tex_load( "res/textures/block.png", false, TEX_BLOCK );
    _tex_load( "res/textures/block_solid.png", false, TEX_BLOCK_SOLID );
    _tex_load( "res/textures/background.png", false, TEX_BACKGROUND );
    _tex_load( "res/textures/paddle.png", true, TEX_PADDLE );
    _tex_load( "res/textures/particle.png", true, TEX_PARTICLE );
}

GLuint rm_shader_get( ShaderType shader_type )
{
    return _shader_table[shader_type];
}

GLuint rm_tex_get( TexType tex_type )
{
    return _tex_table[tex_type];
}