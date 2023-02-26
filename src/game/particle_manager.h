#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include <cglm/cglm.h>

#include "game_object.h"

#define PARTICLE_NUM 500

typedef struct Particle {
    vec2 pos;
    vec2 vel;
    vec4 color;
    float life;
} Particle;

// Initialize particle manager
void pm_init();

// Free particle manager mem
void pm_clean();

// Update all particles
void pm_update(
    float dt,
    GameObject* object,
    size_t new_particles,
    vec2 offset
);

// Render all particles
void pm_draw();

#endif