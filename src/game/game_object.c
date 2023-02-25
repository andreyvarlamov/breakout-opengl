#include "game_object.h"

#include <stdbool.h>

#include <cglm/cglm.h>

#include "../gfx/tex_type.h"

GameObject game_object_create_default()
{
    GameObject game_object;

    glm_vec2_copy( ( vec2 ) { 0.0f, 0.0f }, game_object.position );
    glm_vec2_copy( ( vec2 ) { 1.0f, 1.0f }, game_object.size );
    glm_vec2_copy( ( vec2 ) { 0.0f, 0.0f }, game_object.velocity );
    glm_vec3_copy( ( vec3 ) { 1.0f, 1.0f, 1.0f }, game_object.color );
    game_object.rotation = 0.0f;
    game_object.is_solid = false;
    game_object.destroyed = false;
    game_object.tex_type = TEX_NONE;

    return game_object;
}

GameObject game_object_create(
    vec2 pos,
    vec2 size,
    vec2 velocity,
    vec3 color,
    TexType tex_type
)
{
    GameObject game_object;

    glm_vec2_copy( pos, game_object.position );
    glm_vec2_copy( size, game_object.size );
    glm_vec2_copy( velocity, game_object.velocity );
    glm_vec3_copy( color, game_object.color );
    game_object.rotation = 0.0f;
    game_object.is_solid = false;
    game_object.destroyed = false;
    game_object.tex_type = tex_type;

    return game_object;
}