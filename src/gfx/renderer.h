#ifndef RENDERER_H
#define RENDERER_H

#include <cglm/cglm.h>

#include "tex_type.h"

void quad_init_render_data();

void sprite_draw(
        TexType tex_type,
        vec2 position,
        vec2 size,
        float rotate,
        vec3 color);

#endif