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
    load_texture("res/textures/block.png", false, TEX_BLOCK);
    load_texture("res/textures/block_solid.png", false, TEX_BLOCK_SOLID);
    load_texture("res/textures/background.png", false, TEX_BACKGROUND);

    game_level_load(&game->game_level, "res/levels/one.lvl", width, height / 2);
}

void game_process_input(Game* game, float dt)
{
}

void game_update(Game* game, float dt)
{
}

void game_render(Game* game)
{
    // Render background
    sprite_draw(
        TEX_BACKGROUND,
        (vec2) { 0.0f, 0.0f },
        (vec2) { game->width, game->height },
        0.0f,
        (vec3) { 1.0f, 1.0f, 1.0f });

    // Render game level
    game_level_draw(&game->game_level);
}

void game_clean(Game* game)
{
    game_level_clean(&game->game_level);
}
