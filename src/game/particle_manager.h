#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include <cglm/cglm.h>

#define PARTICLE_NUM 500

typedef struct Particle {
    vec2 pos;
    vec2 vel;
    vec4 color;
    float life;
} Particle;

// Initialize particle manager
void pm_init();

// Update all particles
void pm_update();

// Render all particles
void pm_draw();

// Free particle manager mem
void pm_clean();

#endif