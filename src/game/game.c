#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include <cglm/cglm.h>

#include "../gfx/gfx.h"
#include "../gfx/renderer.h"
#include "../gfx/resource_manager.h"
#include "../gfx/shader_type.h"
#include "../gfx/shader.h"
#include "../gfx/tex_type.h"

void game_init(Game* game, unsigned int width, unsigned int height)
{
    for (int i = 0; i < 1024; ++i)
    {
        game->keys[i] = false;
    }

    game->width = width;
    game->height = height;
    game->state = GAME_ACTIVE;

    GLuint shader = load_shader(
            "res/shaders/quad.vs",
            "res/shaders/quad.fs",
            NULL,
            SHADER_QUAD);

    mat4 projection;
    glm_ortho(
            0.0f,
            (float) game->width,
            (float) game->height,
            0.0f,
            -1.0f,
            1.0f,
            projection);

    shader_use(shader);
    shader_uniform_int(shader, "image", 0);
    shader_uniform_mat4(shader, "projection", projection);

    quad_init_render_data();

    load_texture("res/textures/awesomeface.png", true, TEX_FACE);
}

void game_process_input(Game* game, float dt)
{
}

void game_update(Game* game, float dt)
{
}

void game_render()
{
    sprite_draw(
            TEX_FACE,
            (vec2) { 200.0f, 200.0f },
            (vec2) { 300.0f, 400.0f },
            45.0f,
            (vec3) { 0.0f, 1.0f, 0.0f });
}
