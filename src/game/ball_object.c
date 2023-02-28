#include "ball_object.h"

#include "game_object.h"

BallObject ball_object_create(
    vec2 pos,
    vec2 velocity,
    float radius
)
{
    BallObject ball;

    ball.d = game_object_create(
        pos,
        ( vec2 ) { radius * 2.0f, radius * 2.0f },
        velocity,
        ( vec3 ) { 1.0f, 1.0f, 1.0f },
        TEX_FACE
    );

    ball.radius = radius;
    ball.stuck = true;
    ball.sticky = false;
    ball.pass_through = false;

    return ball;
}

void ball_object_move(
    BallObject* ball,
    float dt,
    unsigned int window_width
)
{
    if ( !ball->stuck )
    {
        // Move the ball
        // -------------
        vec2 scaled_vel;
        glm_vec2_scale( ball->d.velocity, dt, scaled_vel );
        glm_vec2_add( ball->d.position, scaled_vel, ball->d.position );

        // If outside window bounds, correct position, reverse velocity
        // ------------------------------------------------------------
        if ( ball->d.position[0] <= 0.0f )
        {
            ball->d.velocity[0] = -ball->d.velocity[0];
            ball->d.position[0] = 0.0f;
        }
        else if ( ball->d.position[0] + ball->d.size[0] >= window_width )
        {
            ball->d.velocity[0] = -ball->d.velocity[0];
            ball->d.position[0] = window_width - ball->d.size[0];
        }

        if ( ball->d.position[1] <= 0.0f )
        {
            ball->d.velocity[1] = -ball->d.velocity[1];
            ball->d.position[1] = 0.0f;
        }
        // Don't check lower y boundary, because that's game over
    }
}