#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <stdbool.h>

#include <cglm/cglm.h>

#include "../gfx/tex_type.h"

typedef struct GameObject
{
    vec2 position;
    vec2 size;
    vec2 velocity;

    vec3 color;

    float rotation;
    bool is_solid;
    bool destroyed;

    TexType tex_type;
} GameObject;

GameObject game_object_create_default();
GameObject game_object_create(
        vec2 pos,
        vec2 size,
        vec2 velocity,
        vec3 color,
        TexType tex_type);

#endif
