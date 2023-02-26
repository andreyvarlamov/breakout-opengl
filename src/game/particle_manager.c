#include "particle_manager.h"

#include <stdlib.h>
#include <string.h>

#include <cglm/cglm.h>

#include "../gfx/renderer.h"
#include "../gfx/tex_type.h"

vec2* _pos;
vec2* _vel;
vec4* _color;
float* _life;

size_t _prev_unused_particle = 0;
// Get first particle with life <= 0.0f
static size_t _first_unused_particle()
{
    // First search from the particle that was previously the first unused
    // -------------------------------------------------------------------
    for ( size_t i = _prev_unused_particle + 1; i < PARTICLE_NUM; i++ )
    {
        if ( _life[i] <= 0.0f )
        {
            _prev_unused_particle = i;
            return i;
        }
    }

    // If not found, search in the rest of the list
    // --------------------------------------------
    for (size_t i = 0; i < _prev_unused_particle + 1; i++ )
    {
        if ( _life[i] <= 0.0f )
        {
            _prev_unused_particle = i;
            return i;
        }
    }

    // If all particles are in use, override the first one
    // But if this happens a lot, more should be reserved
    // ---------------------------------------------------
    _prev_unused_particle = 0;
    return 0;
}

// Respawn a particle by setting life = 1.0f
static void _respawn_particle( size_t i, GameObject* obj, vec2 offset )
{
    // Set position
    // ------------
    float random = ( ( rand() % 100 ) - 50 ) / 10.0f;
    float rand_color = 0.5f + ( ( rand() % 100 ) / 100.0f );
    vec2 rand_offset;
    glm_vec2_adds( offset, random, rand_offset );
    glm_vec2_add( obj->position, rand_offset, _pos[i] );

    // Set color
    // ---------
    glm_vec4_copy(
        ( vec4 ) { rand_color, rand_color, rand_color, 1.0f },
        _color[i]
    );

    // Set life
    // --------
    _life[i] = 1.0f;

    // Set velocity
    glm_vec2_scale( obj->velocity, 0.1f, _vel[i] );
}

void pm_init()
{
    _pos =   ( vec2* )  calloc( 1, sizeof( vec2 )  * PARTICLE_NUM );
    _vel =   ( vec2* )  calloc( 1, sizeof( vec2 )  * PARTICLE_NUM );
    _color = ( vec4* )  calloc( 1, sizeof( vec4 )  * PARTICLE_NUM );
    _life =  ( float* ) calloc( 1, sizeof( float ) * PARTICLE_NUM );

    // Color should be init to white (1.0f) -> no shade
    for ( size_t i = 0; i < PARTICLE_NUM; i++ )
    {
        glm_vec4_copy(GLM_VEC4_ONE, _color[i]);
    }
}

void pm_clean()
{
    free(_pos);
    free(_vel);
    free(_color);
    free(_life);
}

void pm_update(
    float dt,
    GameObject* object,
    size_t new_particles,
    vec2 offset
)
{
    // Add new particles
    // -----------------
    for ( size_t i = 0; i <  new_particles; i++)
    {
        size_t unused = _first_unused_particle();
        _respawn_particle( unused, object, offset );
    }

    // Update all particles
    // --------------------
    for ( size_t i = 0; i < PARTICLE_NUM; i++ )
    {
        _life[i] -= dt;
        if ( _life[i] > 0.0f )
        {
            // Particle is still alive
            // -----------------------
            vec2 scaled_vel;
            glm_vec2_scale( _vel[i], dt, scaled_vel );
            glm_vec2_sub( _pos[i], scaled_vel, _pos[i] );
            // Fade out by reducing alpha channel
            _color[i][3] -= dt * 2.5f;
        }
    }
}

void pm_draw()
{
    renderer_particle_draw(TEX_PARTICLE, _pos, _color, PARTICLE_NUM);
}