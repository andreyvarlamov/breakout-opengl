#include "particle_manager.h"

#include <stdlib.h>
#include <string.h>

#include <cglm/cglm.h>

#include "../gfx/renderer.h"
#include "../gfx/tex_type.h"

Particle* _particles;

static Particle _particle_create()
{
    // Color should be init to white (1.0f) -> no shade
    Particle par = { 0 };
    glm_vec4_copy( GLM_VEC4_ONE, par.color );
    return par;
}

size_t _prev_unused_particle = 0;
// Get first particle with life <= 0.0f
static size_t _first_unused_particle()
{
    // First search from the particle that was previously the first unused
    // -------------------------------------------------------------------
    for ( size_t i = _prev_unused_particle + 1; i < PARTICLE_NUM; i++ )
    {
        if ( _particles[i].life <= 0.0f )
        {
            _prev_unused_particle = i;
            return i;
        }
    }

    // If not found, search in the rest of the list
    // --------------------------------------------
    for (size_t i = 0; i < _prev_unused_particle + 1; i++ )
    {
        if ( _particles[i].life <= 0.0f )
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
    glm_vec2_add( obj->position, rand_offset, _particles[i].pos );

    // Set color
    // ---------
    glm_vec4_copy(
        ( vec4 ) { rand_color, rand_color, rand_color, 1.0f },
        _particles[i].color
    );

    // Set life
    // --------
    _particles[i].life = 1.0f;

    // Set velocity
    glm_vec2_scale( obj->velocity, 0.1f, _particles[i].vel );
}

void pm_init()
{
    _particles = ( Particle* ) calloc( 1, sizeof (Particle ) * PARTICLE_NUM );

    for ( size_t i = 0; i < PARTICLE_NUM; i++ )
    {
        Particle par = _particle_create();
        memcpy( &_particles[i], &par, sizeof( Particle ) );
    }
}

void pm_clean()
{
    free(_particles);
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
        _particles[i].life -= dt;
        if ( _particles[i].life > 0.0f )
        {
            // Particle is still alive
            // -----------------------
            vec2 scaled_vel;
            glm_vec2_scale( _particles[i].vel, dt, scaled_vel );
            glm_vec2_sub( _particles[i].pos, scaled_vel, _particles[i].pos );
            // Fade out by reducing alpha channel
            _particles[i].color[3] -= dt * 2.5f;
        }
    }
}

void pm_draw()
{
    renderer_particle_draw_prepare( TEX_PARTICLE );

    for ( size_t i = 0; i < PARTICLE_NUM; i++ )
    {
        if ( _particles[i].life > 0.0f )
        {
            renderer_particle_draw_do( _particles[i].pos, _particles[i].color );
        }
    }

    renderer_particle_draw_end();
}