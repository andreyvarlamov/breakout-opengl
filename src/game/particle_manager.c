#include "particle_manager.h"

#include <string.h>

#include <cglm/cglm.h>

Particle* _particles;

Particle _particle_create()
{
    Particle par = { 0 };
    glm_vec2_copy( GLM_VEC2_ONE, par.color );
    return par;
}

void pm_init()
{
    _particles = ( Particle* ) calloc(1, sizeof( Particle ) * PARTICLE_NUM );

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