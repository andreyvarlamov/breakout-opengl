#include "renderer.h"

#include <cglm/cglm.h>

#include "gfx.h"
#include "shader_type.h"
#include "shader.h"
#include "tex_type.h"
#include "texture.h"
#include "resource_manager.h"

GLuint _quad_vao;

void quad_init_render_data()
{
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

    glGenVertexArrays(1, &_quad_vao);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(_quad_vao);
    glEnableVertexAttribArray(0);

    /* A Vertex Array Object however, stores vertexAttribPointer data and EBO
       binds, but does not store VBO binds. What it does store is, in what
       buffer it can find its vertex data after a call to
       glVertexAttribPointer, that stores the currently bound VBO as the data
       resource for that specific vertex attribute; that is stored in the VAO.
       So we're free to unbind the VBO as long as the corresponding calls to
       glVertexAttribPointer are made that require that VBO. */
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
            4 * sizeof(float), (void*) 0);
    // Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind _quad_vao
    glBindVertexArray(0);
}

void sprite_draw(
        TexType tex_type,
        vec2 position,
        vec2 size,
        float rotate,
        vec3 color)
{
    GLuint shader_id = get_shader(SHADER_QUAD);
    shader_use(shader_id);

    mat4 model;
    glm_mat4_identity(model);

    glm_translate(model, (vec3) { position[0], position[1], 0.0f });
    glm_translate(model, (vec3) { 0.5f * size[0], 0.5f * size[1], 0.0f });
    glm_rotate(model, rotate, (vec3) { 0.0f, 0.0f, 1.0f });
    glm_translate(model, (vec3) { -0.5f * size[0], -0.5f * size[1], 0.0f });

    glm_scale(model, (vec3) { size[0], size[1], 1.0f });

    shader_uniform_mat4(shader_id, "model", model);
    shader_uniform_vec3(shader_id, "color", color);
}
