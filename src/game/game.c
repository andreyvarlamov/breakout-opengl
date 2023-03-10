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
#include "particle_manager.h"
#include "post_proc.h"
#include "powup_manager.h"

#define PLAYER_WIDTH 100.0f
#define PLAYER_HEIGHT 20.0f
#define PLAYER_VELOCITY 800.0f
//#define PLAYER_VELOCITY 500.0f
#define BALL_RADIUS 12.5f
#define BALL_INIT_VEL (vec2) { 125.0f, -450.0f }
//#define BALL_INIT_VEL (vec2) { 100.0f, -350.0f }
//#define BALL_INIT_VEL (vec2) { 0.0f, -300.0f }

void game_init( Game* game, unsigned int width, unsigned int height )
{
    // Set initial game params
    // -----------------------
    game->width = width;
    game->height = height;
    game->state = GAME_ACTIVE;

    // Load resources from file into OpenGL
    // ------------------------------------
    rm_shader_load_all();
    rm_tex_load_all();

    // Prepare render VAOs and shaders
    // -------------------------------
    renderer_quad_init( width, height );
    renderer_particle_init( width, height );

    // Prepare off-screen multisampled framebuffer
    // And scene VAOs and shaders
    // -------------------------------------------
    renderer_framebuffer_init( width, height );
    renderer_scene_init();

    // Init level data
    // ---------------
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

    // Init power up holder
    // --------------------
    powup_init( &game->powup_man );

    // Init particle manager
    // ---------------------
    pm_init();
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

// TODO: How do I move this logic, so that powup manager manages what powups do
static void _activate_powup( Game* game, PowupType pu_type )
{
    powup_effect_set( &game->powup_man, pu_type );

    if ( pu_type == POWUP_SPEED )
    {
        glm_vec2_scale( game->ball.d.velocity, 1.2f, game->ball.d.velocity );
    }
    else if ( pu_type == POWUP_STICKY )
    {
        game->ball.sticky = true;
        glm_vec3_copy( ( vec3 ) { 1.0f, 0.5f, 1.0f }, game->player.color );
    }
    else if ( pu_type == POWUP_PASS_THROUGH )
    {
        game->ball.pass_through = true;
        glm_vec3_copy( ( vec3 ) { 1.0f, 0.5f, 0.5f }, game->ball.d.color );
    }
    else if ( pu_type == POWUP_PAD_SIZE_INCREASE )
    {
        game->player.size[0] += 50.0f;
    }
    else if ( pu_type == POWUP_CONFUSE )
    {
        post_proc_set_confuse( true );
    }
    else if ( pu_type == POWUP_CHAOS )
    {
        post_proc_set_chaos( true );
    }
}

static void _check_powup_eff_timeout( Game* game )
{
    if ( powup_effect_get_to_disable( &game->powup_man, POWUP_SPEED ) )
    {
        // float scale_down = 1.0f / 1.2f;
        // glm_vec2_scale(
        //     game->ball.d.velocity,
        //     scale_down,
        //     game->ball.d.velocity
        // );
    }
    if ( powup_effect_get_to_disable( &game->powup_man, POWUP_STICKY ) )
    {
        game->ball.sticky = false;
        glm_vec3_copy( GLM_VEC3_ONE, game->player.color );
    }
    if ( powup_effect_get_to_disable( &game->powup_man, POWUP_PASS_THROUGH ) )
    {
        game->ball.pass_through = false;
        glm_vec3_copy( GLM_VEC3_ONE, game->ball.d.color );
    }
    if ( powup_effect_get_to_disable( &game->powup_man, POWUP_PAD_SIZE_INCREASE ) )
    {
        // glm_vec2_copy(
        //     ( vec2 ) { PLAYER_WIDTH, PLAYER_HEIGHT },
        //     game->player.size
        // );
    }
    if ( powup_effect_get_to_disable( &game->powup_man, POWUP_CONFUSE ) )
    {
        post_proc_set_confuse( false );
    }
    if ( powup_effect_get_to_disable( &game->powup_man, POWUP_CHAOS ) )
    {
        post_proc_set_chaos( false );
    }
}

static float _shake_time = 0.0f;

static void _do_collisions( Game* game )
{
    // Check collisions between bricks and ball
    // ----------------------------------------
    GameObject* bricks = game->game_levels[game->current_level].bricks;
    size_t brick_count = game->game_levels[game->current_level].bricks_tot;

    bool vel_changed = false;

    for ( size_t i = 0; i < brick_count; i++ )
    {
        if ( !bricks[i].destroyed )
        {
            // Collision detection
            // -------------------
            Collision col = col_check_circ_rect( game->ball, bricks[i] );
            if ( col.is )
            {
                // Destroy block if not solid
                // --------------------------
                if ( !bricks[i].is_solid )
                {
                    bricks[i].destroyed = true;
                    powup_object_respawn_random(
                        &game->powup_man,
                        bricks[i].position
                    );
                }
                else
                {
                    // If block is solid, enable shake effect
                    _shake_time = 0.05f;
                    post_proc_set_shake( true );
                }

                // Collision resolution
                // --------------------
                // Only resolve collision if the ball doesn't have pass-through
                // effect, unless it's solid, then ignore pass-through effect
                if ( !game->ball.pass_through || bricks[i].is_solid )
                {
                    if ( col.dir == LEFT || col.dir == RIGHT ) // horizontal col
                    {
                        if (!vel_changed)
                        {
                            // Only want to invert velocity once per frame
                            // But the rest of the collision resolution can
                            // happen multiple times per frame, as we don't want
                            // objects to overlap
                            game->ball.d.velocity[0] =
                                -game->ball.d.velocity[0];
                            vel_changed = true;
                        }

                        float penetration =
                            game->ball.radius - fabs( col.diff[0] );

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
                        if (!vel_changed)
                        {
                            game->ball.d.velocity[1] =
                                -game->ball.d.velocity[1];
                            vel_changed = true;
                        }

                        float penetration =
                            game->ball.radius - fabs( col.diff[1] );

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

            game->ball.stuck = game->ball.sticky;
        }
    }

    // Check collisions between player and powerups
    // --------------------------------------------
    for ( size_t i = 0; i < POWUP_OBJECT_NUM; i++ )
    {
        if ( !game->powup_man.obj[i].destroyed )
        {
            if ( game->powup_man.obj[i].position[1] >= game->height )
            {
                // Reached the bottom, destroy
                game->powup_man.obj[i].destroyed = true;
            }
            else
            {
                bool col = col_check_rects(
                    game->player,
                    game->powup_man.obj[i]
                );

                if ( col )
                {
                    game->powup_man.obj[i].destroyed = true;
                    _activate_powup( game, game->powup_man.pu_types[i] );
                }
            }

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

    // Check if game over
    // ------------------
    if ( game->ball.d.position[1] > game->height )
    {
        game_reset_levels( game );
        game_reset_player_and_ball( game );
        powup_init( &game->powup_man );
        post_proc_reset_all();
    }

    // Update power up objects
    // -----------------------
    powup_update( &game->powup_man, dt );
    _check_powup_eff_timeout( game );
    
    // Update particles
    // ----------------
    pm_update(
        dt,
        &game->ball.d,
        2, // 2 new particles per frame
        ( vec2 ) { game->ball.radius / 2.0f, game->ball.radius / 2.0f }
    );

    // Update effects
    // --------------
    if (_shake_time > 0.0f)
    {
        _shake_time -= dt;
        if ( _shake_time <= 0.0f )
        {
            post_proc_set_shake( false );
        }
    }
}

void game_render( Game* game )
{
    // Everything after will be rendered onto
    // the scene framebuffer
    // --------------------------------------
    renderer_framebuffer_bind();

    // Render background
    // -----------------
    renderer_quad_draw(
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
    renderer_quad_draw(
        game->player.tex_type,
        game->player.position,
        game->player.size,
        game->player.rotation,
        game->player.color
    );

    // Render power up objects
    // -----------------------
    powup_object_draw( &game->powup_man );

    // Render particle effects
    // -----------------------
    pm_draw();

    // Render the ball
    // ---------------
    renderer_quad_draw(
        game->ball.d.tex_type,
        game->ball.d.position,
        game->ball.d.size,
        game->ball.d.rotation,
        game->ball.d.color
    );

    // Everything after will be rendered
    // onto the default on-screen framebuffer
    // --------------------------------------
    renderer_framebuffer_unbind();

    // Send post-processing settings to scene shader
    // ---------------------------------------------
    post_proc_send_uniforms();

    // Render the actual scene onto the screen
    // ---------------------------------------
    renderer_scene_draw();
}

void game_clean( Game* game )
{
    for ( size_t i = 0; i < GAME_LEVEL_NUM; i++ )
    {
        game_level_clean( &game->game_levels[i] );
    }

    pm_clean();
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