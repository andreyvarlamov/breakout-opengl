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

#define PLAYER_WIDTH 100.0f
#define PLAYER_HEIGHT 20.0f
#define PLAYER_VELOCITY 500.0f

void game_init( Game* game, unsigned int width, unsigned int height )
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
        SHADER_QUAD
    );

    mat4 projection;
    glm_ortho(
        0.0f,
        (float) game->width,
        (float) game->height,
        0.0f,
        -1.0f,
        1.0f,
        projection
    );

    shader_use( shader );
    shader_uniform_int( shader, "image", 0 );
    shader_uniform_mat4( shader, "projection", projection );

    quad_init_render_data();

    load_texture( "res/textures/awesomeface.png", true, TEX_FACE );
    load_texture( "res/textures/block.png", false, TEX_BLOCK );
    load_texture( "res/textures/block_solid.png", false, TEX_BLOCK_SOLID );
    load_texture( "res/textures/background.png", false, TEX_BACKGROUND );
    load_texture( "res/textures/paddle.png", true, TEX_PADDLE );

    game_level_load(
        &game->game_levels[0],
        "res/levels/one.lvl",
        width,
        height / 2
    );

    game_level_load(
        &game->game_levels[1],
        "res/levels/two.lvl",
        width,
        height / 2
    );

    game_level_load(
        &game->game_levels[2],
        "res/levels/three.lvl",
        width,
        height / 2
    );

    game_level_load(
        &game->game_levels[3],
        "res/levels/four.lvl",
        width,
        height / 2
    );

    game->current_level = 0;

    game->player = game_object_create(
        (vec2) { (width / 2.0f) - (PLAYER_WIDTH / 2.0f), height - PLAYER_HEIGHT },
        (vec2) { PLAYER_WIDTH, PLAYER_HEIGHT },
        (vec2) { 0.0f, 0.0f },
        (vec3) { 1.0f, 1.0f, 1.0f },
        TEX_PADDLE
    );
}

void game_process_input( Game* game, float dt )
{
    if (game->state == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;

        if (game->keys[GLFW_KEY_A])
        {
            game->player.position[0] -= velocity;
            if (game->player.position[0] < 0.0f)
            {
                game->player.position[0] = 0.0f;
            }
        }
        if (game->keys[GLFW_KEY_D])
        {
            game->player.position[0] += velocity;
            if (game->player.position[0] > game->width - game->player.size[0])
            {
                game->player.position[0] = game->width - game->player.size[0];
            }
        }
    }
}

void game_update( Game* game, float dt )
{
}

void game_render( Game* game )
{
    // Render background
    sprite_draw(
        TEX_BACKGROUND,
        (vec2) { 0.0f, 0.0f },
        (vec2) { game->width, game->height },
        0.0f,
        (vec3) { 1.0f, 1.0f, 1.0f }
    );

    // Render game level
    game_level_draw( &game->game_levels[game->current_level] );

    // Render player
    sprite_draw(
        game->player.tex_type,
        game->player.position,
        game->player.size,
        game->player.rotation,
        game->player.color
    );
}

void game_clean( Game* game )
{
    game_level_clean( &game->game_levels[game->current_level] );
}
