#include "game.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <cglm/cglm.h>

#include "../gfx/gfx.h"
#include "../gfx/renderer.h"
#include "../gfx/resource_manager.h"
#include "../gfx/shader_type.h"
#include "../gfx/shader.h"
#include "../gfx/tex_type.h"
#include "collisions.h"

#define PLAYER_WIDTH 100.0f
#define PLAYER_HEIGHT 20.0f
#define PLAYER_VELOCITY 500.0f
#define BALL_RADIUS 12.5f
#define BALL_INIT_VEL (vec2) { 100.0f, -350.0f }
//#define BALL_INIT_VEL (vec2) { 0.0f, -100.0f }

void game_init( Game* game, unsigned int width, unsigned int height )
{
    for ( int i = 0; i < 1024; ++i )
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
        ( float ) game->width,
        ( float ) game->height,
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

    game->current_level = STARTING_LEVEL;

    game_reset_player_and_ball( game );
}

void game_process_input( Game* game, float dt )
{
    if ( game->state == GAME_ACTIVE )
    {
        float velocity = PLAYER_VELOCITY * dt;

        if ( game->keys[GLFW_KEY_A] )
        {
            if ( game->player.position[0] >= 0.0f )
            {
                game->player.position[0] -= velocity;

                if ( game->ball.stuck )
                {
                    game->ball.d.position[0] -= velocity;
                }
            }
        }
        if ( game->keys[GLFW_KEY_D] )
        {
            if ( game->player.position[0] <= game->width - game->player.size[0] )
            {
                game->player.position[0] += velocity;

                if ( game->ball.stuck )
                {
                    game->ball.d.position[0] += velocity;
                }
            }
        }

        if ( game->keys[GLFW_KEY_SPACE] )
        {
            game->ball.stuck = false;
        }
    }
}

void _do_collisions( Game* game )
{
    // Check collisions between bricks and ball
    // ----------------------------------------
    GameObject* bricks = game->game_levels[game->current_level].bricks;
    size_t brick_count = game->game_levels[game->current_level].bricks_tot;

    for ( size_t i = 0; i < brick_count; i++ )
    {
        if ( !bricks[i].destroyed )
        {
            // Collision detection
            // -------------------
            Collision col = col_check_circ_rect( game->ball, bricks[i] );
            if ( col.is && !game->ball.resolving_collision )
            {
                game->ball.resolving_collision = true;

                // Destroy block if not solid
                // --------------------------
                if ( !bricks[i].is_solid )
                {
                    bricks[i].destroyed = true;
                }

                // Collision resolution
                // --------------------
                if ( col.dir == LEFT || col.dir == RIGHT ) // horizontal col
                {
                    game->ball.d.velocity[0] = -game->ball.d.velocity[0];

                    float penetration = game->ball.radius - fabs( col.diff[0] );

                    if ( col.dir == LEFT )
                    {
                        game->ball.d.position[0] -= penetration;
                    }
                    else
                    {
                        game->ball.d.position[0] += penetration;
                    }
                }
                else // vertical col
                {
                    game->ball.d.velocity[1] = -game->ball.d.velocity[1];

                    float penetration = game->ball.radius - fabs( col.diff[1] );

                    if ( col.dir == UP )
                    {
                        game->ball.d.position[1] -= penetration;
                    }
                    else
                    {
                        game->ball.d.position[1] += penetration;
                    }
                }
            }
            else if ( !col.is && game->ball.resolving_collision )
            {
                game->ball.resolving_collision = false;
            }
        }
    }

    // Check collisions between player paddle and ball
    // -----------------------------------------------
    if ( !game->ball.stuck )
    {
        Collision col = col_check_circ_rect( game->ball, game->player );

        if (col.is)
        {
            // The further the ball hits the paddle from its center, the
            // stronger its horizontal velocity should be.
            float paddle_center =
                game->player.position[0] + game->player.size[0] / 2.0f;
            float distance =
                ( game->ball.d.position[0] + game->ball.radius )
                - paddle_center;
            float percent = distance / ( game->player.size[0] / 2.0f );

            float strength = 2.0f;

            vec2 old_vel;
            glm_vec2_copy(game->ball.d.velocity, old_vel);
            float old_length = glm_vec2_distance ( GLM_VEC2_ZERO, old_vel );

            game->ball.d.velocity[0] = BALL_INIT_VEL[0] * percent * strength;

            // Because the paddle is always hit from the top, we can just
            // set the velocity to be directed upward.
            // To avoid the sticky paddle issue (when collision is detected too
            // late, and the ball's center is already inside the paddle)
            game->ball.d.velocity[1] = -1.0f * fabs( game->ball.d.velocity[1] );

            // The length of the old vector should be preserved,
            // so the overall speed is the same
            glm_vec2_normalize( game->ball.d.velocity );
            glm_vec2_scale(
                game->ball.d.velocity,
                old_length,
                game->ball.d.velocity
            );
        }
    }
}

void game_update( Game* game, float dt )
{
    // Update objects
    // --------------
    ball_object_move( &game->ball, dt, game->width );

    // Check for collisions
    // --------------------
    _do_collisions( game );

    // Check if the ball reached the bottom edge
    if ( game->ball.d.position[1] > game->height )
    {
        game_reset_levels( game );
        game_reset_player_and_ball( game );
    }
}

void game_render( Game* game )
{
    // Render background
    // -----------------
    sprite_draw(
        TEX_BACKGROUND,
        (vec2) { 0.0f, 0.0f },
        (vec2) { game->width, game->height },
        0.0f,
        (vec3) { 1.0f, 1.0f, 1.0f }
    );

    // Render game level
    // -----------------
    game_level_draw( &game->game_levels[game->current_level] );

    // Render player paddle
    // --------------------
    sprite_draw(
        game->player.tex_type,
        game->player.position,
        game->player.size,
        game->player.rotation,
        game->player.color
    );

    // Render the ball
    // ---------------
    sprite_draw(
        game->ball.d.tex_type,
        game->ball.d.position,
        game->ball.d.size,
        game->ball.d.rotation,
        game->ball.d.color
    );
}

void game_clean( Game* game )
{
    for ( size_t i = 0; i < GAME_LEVEL_NUM; i++ )
    {
        game_level_clean( &game->game_levels[i] );
    }
}

void game_reset_levels( Game* game )
{
    // TODO: not a good way to do this
    if ( game->current_level == 0 )
    {
        game_level_load(
            &game->game_levels[0],
            "res/levels/one.lvl",
            game->width,
            game->height / 2
        );
    }
    else if ( game->current_level == 1 )
    {
        game_level_load(
            &game->game_levels[1],
            "res/levels/two.lvl",
            game->width,
            game->height / 2
        );
    }
    else if ( game->current_level == 2 )
    {
        game_level_load(
            &game->game_levels[2],
            "res/levels/three.lvl",
            game->width,
            game->height / 2
        );
    }
    else if ( game->current_level == 3 )
    {
        game_level_load(
            &game->game_levels[3],
            "res/levels/four.lvl",
            game->width,
            game->height / 2
        );
    }

}

void game_reset_player_and_ball( Game* game )
{
    vec2 player_pos = {
        ( game->width / 2.0f ) - ( PLAYER_WIDTH / 2.0f ),
        game->height - PLAYER_HEIGHT
    };

    game->player = game_object_create(
        player_pos,
        ( vec2 ) { PLAYER_WIDTH, PLAYER_HEIGHT },
        ( vec2 ) { 0.0f, 0.0f },
        ( vec3 ) { 1.0f, 1.0f, 1.0f },
        TEX_PADDLE
    );

    vec2 ball_pos;
    glm_vec2_add(
        player_pos,
        ( vec2 ) { ( PLAYER_WIDTH / 2.0f ) - BALL_RADIUS, -BALL_RADIUS * 2.0f },
        ball_pos
    );

    game->ball = ball_object_create(
        ball_pos,
        BALL_INIT_VEL,
        BALL_RADIUS
    );
}