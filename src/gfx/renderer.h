#ifndef RENDERER_H
#define RENDERER_H

#include <cglm/cglm.h>

#include "tex_type.h"

/***************************************************/
/*                  QUAD RENDERING                 */
/***************************************************/

// Prepare quad VAO and shader data
void renderer_quad_init( unsigned int game_width, unsigned int game_height );

void renderer_quad_draw(
    TexType tex_type,
    vec2 position,
    vec2 size,
    float rotate,
    vec3 color
);

/***************************************************/
/*                PARTICLE RENDERING               */
/***************************************************/

// Prepare particle VAO and shader data
void renderer_particle_init();

void renderer_particle_draw(
    TexType tex_type,
    vec2 positions[],
    vec4 colors[],
    size_t count
);

#endif
