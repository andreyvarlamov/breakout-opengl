#include "renderer.h"

#include <cglm/cglm.h>

#include "gfx.h"
#include "shader_type.h"
#include "shader.h"
#include "tex_type.h"
#include "texture.h"
#include "resource_manager.h"

GLuint _quad_vao;
GLuint _particle_vao;

GLuint _msfbo;
GLuint _fbo;
GLuint _rbo;

GLuint _framebuffer_tex;

unsigned int _game_width;
unsigned int _game_height;

/******************************************************************************
 *                                FRAMEBUFFER                                 *
 ******************************************************************************/

void renderer_framebuffer_init(
    unsigned int game_width,
    unsigned int game_height
)
{
    _game_width = game_width;
    _game_height = game_height;
    // Initialize renderbuffer / framebuffer objects
    // ---------------------------------------------
    glGenFramebuffers ( 1, &_msfbo );
    glGenFramebuffers ( 1, &_fbo   );
    glGenRenderbuffers( 1, &_rbo   );

    // Initialize render buffer storage with a multisampled
    // color buffer (don't need a depth/stencil buffer)
    // ----------------------------------------------------
    glBindFramebuffer ( GL_FRAMEBUFFER,  _msfbo );
    glBindRenderbuffer( GL_RENDERBUFFER, _rbo   );

    glRenderbufferStorageMultisample( // allocate storage for render buffer obj
        GL_RENDERBUFFER,
        4,
        GL_RGB,
        game_width,
        game_height
    );

    glFramebufferRenderbuffer( // attach MS render buffer object to framebuffer
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_RENDERBUFFER,
        _rbo
    );

    if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
    {
        printf("ERROR::POSTPROCESSOR: Failed to initialize MSFBO\n");
    }

    // Also initialize the FBO/texture to blit multisampled color-buffer to;
    // used for shader operations (for post-processing)
    // ---------------------------------------------------------------------
    glBindFramebuffer( GL_FRAMEBUFFER, _fbo);
    _framebuffer_tex = texture2d_generate(
        game_width,
        game_height,
        false,
        NULL
    );

    glFramebufferTexture2D(    // attach texture to framebuffer as its color
        GL_FRAMEBUFFER,        // attachment
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        _framebuffer_tex,
        0
    );

    if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
    {
        printf("ERROR::POSTPROCESSOR: Failed to initialize FBO\n");
    }

    glBindFramebuffer ( GL_FRAMEBUFFER,  0 );
    glBindRenderbuffer( GL_RENDERBUFFER, 0 );
}

void renderer_framebuffer_bind()
{
    glBindFramebuffer( GL_FRAMEBUFFER, _msfbo );
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
}

void renderer_framebuffer_unbind()
{
    // Now resolve multisampled color-buffer into ntermediate FBO
    // to store the texture
    glBindFramebuffer( GL_READ_FRAMEBUFFER, _msfbo );
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, _fbo );
    glBlitFramebuffer(
        0, 0, _game_width, _game_height,
        0, 0, _game_width, _game_height,
        GL_COLOR_BUFFER_BIT, GL_NEAREST
    );
    // Bind both READ and WRITE framebuffer to default framebuffer
    // Draws after this will be on-screen
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void renderer_framebuffer_draw_init();

void renderer_framebuffer_draw();

/******************************************************************************
 *                               QUAD RENDERING                               *
 ******************************************************************************/

void renderer_quad_init( unsigned int game_width, unsigned int game_height )
{
    // Prepare the VAO
    // ---------------
    GLuint VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
    };

    glGenVertexArrays( 1, &_quad_vao ); // generate a vao
    glGenBuffers( 1, &VBO ); // generate a vbo

    glBindBuffer( GL_ARRAY_BUFFER, VBO ); // bind the vbo
    glBufferData( // fill the vbo with data from vertices
        GL_ARRAY_BUFFER,
        sizeof( vertices ),
        vertices,
        GL_STATIC_DRAW
    );

    glBindVertexArray( _quad_vao ); // bind the vao

    // set an attribute for the particle mesh
    glEnableVertexAttribArray( 0 );
    /* A Vertex Array Object however, stores vertexAttribPointer data and EBO
     * binds, but does not store VBO binds. What it does store is, in what
     * buffer it can find its vertex data after a call to
     * glVertexAttribPointer, that stores the currently bound VBO as the data
     * resource for that specific vertex attribute; that is stored in the VAO.
     * So we're free to unbind the VBO as long as the corresponding calls to
     * glVertexAttribPointer are made that require that VBO. */
    glVertexAttribPointer( // how to interpret data from vbo
        0, 4, GL_FLOAT, GL_FALSE,
        4 * sizeof( float ), ( void* ) 0
    );

    // Unbind VBO & _quad_vao
    // ----------------------
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );

    // Provide constant uniforms to quad shader
    // ----------------------------------------
    GLuint shader = rm_shader_get( SHADER_QUAD );
    shader_use( shader );

    // Get the shader to use texture loaded under samplerID 0
    shader_uniform_int( shader, "image", 0 );

    // Provide the projection matrix to the shader
    mat4 projection;
    glm_ortho(
        0.0f,
        ( float ) game_width,
        ( float ) game_height,
        0.0f,
        -1.0f,
        1.0f,
        projection
    );
    shader_uniform_mat4( shader, "projection", projection );

    // Unload the shader
    shader_use ( 0 );
}

void renderer_quad_draw(
    TexType tex_type,
    vec2 position,
    vec2 size,
    float rotate,
    vec3 color
)
{
    GLuint shader_id = rm_shader_get( SHADER_QUAD );
    shader_use( shader_id );

    mat4 model;
    glm_mat4_identity( model );

    glm_translate( model, ( vec3 ) { position[0], position[1], 0.0f } );
    glm_translate( model, ( vec3 ) { 0.5f * size[0], 0.5f * size[1], 0.0f } );
    glm_rotate( model, rotate, ( vec3 ) { 0.0f, 0.0f, 1.0f } );
    glm_translate( model, ( vec3 ) { -0.5f * size[0], -0.5f * size[1], 0.0f } );

    glm_scale( model, ( vec3 ) { size[0], size[1], 1.0f } );

    shader_uniform_mat4( shader_id, "model", model );
    shader_uniform_vec3( shader_id, "sprite_color", color );

    glActiveTexture( GL_TEXTURE0 );
    texture2d_bind( rm_tex_get( tex_type ) );

    glBindVertexArray( _quad_vao );
    glDrawArrays( GL_TRIANGLES, 0, 6 );
    glBindVertexArray( 0 );

    // Unload the shader
    shader_use ( 0 );
}

/******************************************************************************
 *                             PARTICLE RENDERING                             *
 ******************************************************************************/

void renderer_particle_init( unsigned int game_width, unsigned int game_height )
{
    // Prepare particle VAO
    // --------------------
    GLuint VBO;

    float particle_vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
    };

    glGenVertexArrays( 1, &_particle_vao ); // generate a vao
    glGenBuffers( 1, &VBO ); // generate a vbo

    glBindBuffer( GL_ARRAY_BUFFER, VBO ); // bind the vbo
    glBufferData( // fill the vbo with data from particle_vertices
        GL_ARRAY_BUFFER,
        sizeof( particle_vertices ),
        particle_vertices,
        GL_STATIC_DRAW
    );

    glBindVertexArray( _particle_vao ); // bind the vao

    // set an attribute for the particle mesh
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( // how to inerpret data from vbo
            0, 4, GL_FLOAT, GL_FALSE,
            4 * sizeof( float ), ( void* ) 0
    );

    // Unbind VBO & _particle_vao
    // --------------------------
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );

    // Provide constant uniforms to particle shader
    // ----------------------------------------
    GLuint shader = rm_shader_get( SHADER_PARTICLE );
    shader_use( shader );

    // Get the shader to use texture loaded under samplerID 0
    shader_uniform_int( shader, "image", 0 );

    // Provide the projection matrix to the shader
    mat4 projection;
    glm_ortho(
        0.0f,
        ( float ) game_width,
        ( float ) game_height,
        0.0f,
        -1.0f,
        1.0f,
        projection
    );
    shader_uniform_mat4( shader, "projection", projection );

    // Unload the shader
    shader_use ( 0 );
}

void renderer_particle_draw_prepare( TexType tex_type )
{
    GLuint shader_id = rm_shader_get( SHADER_PARTICLE );
    shader_use( shader_id );

    // Use additive blending to give it a 'glow' effect
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    glBindVertexArray( _particle_vao );
    GLuint tex = rm_tex_get( tex_type );
    glActiveTexture( GL_TEXTURE0 );
    texture2d_bind( tex );
}

void renderer_particle_draw_do( vec2 position, vec4 color )
{
    GLuint shader_id = rm_shader_get( SHADER_PARTICLE );
    shader_uniform_vec2( shader_id, "offset", position );
    shader_uniform_vec4( shader_id, "particle_color", color );
    glDrawArrays ( GL_TRIANGLES, 0, 6 );
}

void renderer_particle_draw_end()
{
    // Unbind VAO
    glBindVertexArray( 0 );

    // Reset to default blending mode
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    // Unload the shader
    shader_use ( 0 );
}