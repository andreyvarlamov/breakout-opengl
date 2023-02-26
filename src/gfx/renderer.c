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

/***************************************************/
/*                  QUAD RENDERING                 */
/***************************************************/

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
    /*************************************************************************
       A Vertex Array Object however, stores vertexAttribPointer data and EBO
       binds, but does not store VBO binds. What it does store is, in what
       buffer it can find its vertex data after a call to
       glVertexAttribPointer, that stores the currently bound VBO as the data
       resource for that specific vertex attribute; that is stored in the VAO.
       So we're free to unbind the VBO as long as the corresponding calls to
       glVertexAttribPointer are made that require that VBO. 
       ***********************************************************************/
    glVertexAttribPointer( // how to interpret data from vbo
        0, 4, GL_FLOAT, GL_FALSE,
        4 * sizeof( float ), ( void* ) 0
    );

    // Unbind VBO & _quad_vao
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
}

/***************************************************/
/*                PARTICLE RENDERING               */
/***************************************************/

void renderer_particle_init()
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
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );

    // Provide constant uniforms to particle shader
    // ----------------------------------------
    GLuint shader = rm_shader_get( SHADER_PARTICLE );
    shader_use( shader );

    // Get the shader to use texture loaded under samplerID 0
    shader_uniform_int( shader, "image", 0 );

    // Unload the shader
    shader_use ( 0 );
}

void renderer_particle_draw(
    TexType tex_type,
    vec2 positions[],
    vec4 colors[],
    size_t count
)
{
    GLuint shader_id = rm_shader_get( SHADER_PARTICLE );
    shader_use( shader_id );

    // Use additive blending to give it a 'glow' effect
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    glBindVertexArray( _particle_vao );
    GLuint tex = rm_tex_get( tex_type );
    glActiveTexture( GL_TEXTURE0 );
    texture2d_bind( tex );

    for ( size_t i = 0; i < count; i++ )
    {
        shader_uniform_vec2( shader_id, "offset", positions[i] );
        shader_uniform_vec4( shader_id, "particle_color", colors[i] );
        glDrawArrays ( GL_TRIANGLES, 0, 6 );
    }

    glBindVertexArray( 0 );

    // Reset to default blending mode
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}