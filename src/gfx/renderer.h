#ifndef RENDERER_H
#define RENDERER_H

#include <cglm/cglm.h>

#include "tex_type.h"

/***************************************************/
/*                  QUAD RENDERING                 */
/***************************************************/

void quad_init_render_data();

void sprite_draw(
    TexType tex_type,
    vec2 position,
    vec2 size,
    float rotate,
    vec3 color
);

/***************************************************/
/*                PARTICLE RENDERING               */
/***************************************************/

void particle_init_render_data();

void particle_draw(
    TexType tex_type,
    vec2 positions[],
    vec4 colors[],
    size_t count
);

#endif
