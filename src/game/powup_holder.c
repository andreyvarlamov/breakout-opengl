#include "powup_holder.h"

#include <cglm/cglm.h>

#include "../gfx/renderer.h"

void powup_respawn( PowupHolder* powup_holder, PowupType powup_type, vec2 pos )
{
    size_t unused = -1;
    for ( size_t i = 0; i < POWUP_OBJECT_NUM; i++ )
    {
        if ( powup_holder->powups[i].destroyed )
        {
            unused = i;
            break;
        }
    }

    if ( unused == ( size_t ) -1 )
    {
        // No unused slot found, skip
        return;
    }

    if ( ( ( unsigned int ) powup_type ) >=  POWUP_COUNT )
    {
        // Invalid powup type, skip
        return;
    }

    powup_holder->powups[unused].destroyed = false;
    powup_holder->powup_types[unused] = powup_type;
    glm_vec2_copy( pos,               powup_holder->powups[unused].position );
    glm_vec2_copy( POWUP_OBJECT_SIZE, powup_holder->powups[unused].size );
    glm_vec2_copy( POWUP_OBJECT_VEL,  powup_holder->powups[unused].velocity );

    if ( powup_type == POWUP_SPEED )
    {
        glm_vec3_copy(
            POWUP_COL_SPEED,
            powup_holder->powups[unused].color
        );
        powup_holder->powups[unused].tex_type = TEX_POWUP_SPEED;
    }
    else if ( powup_type == POWUP_STICKY )
    {
        glm_vec3_copy(
            POWUP_COL_STICKY,
            powup_holder->powups[unused].color
        );
        powup_holder->powups[unused].tex_type = TEX_POWUP_STICKY;
    }
    else if ( powup_type == POWUP_PASS_THROUGH )
    {
        glm_vec3_copy(
            POWUP_COL_PASS_THROUGH,
            powup_holder->powups[unused].color
        );
        powup_holder->powups[unused].tex_type = TEX_POWUP_PASS_THROUGH;
    }
    else if ( powup_type == POWUP_PAD_SIZE_INCREASE )
    {
        glm_vec3_copy(
            POWUP_COL_PAD_SIZE_INCREASE,
            powup_holder->powups[unused].color
        );
        powup_holder->powups[unused].tex_type = TEX_POWUP_PAD_SIZE_INCREASE;
    }
    else if ( powup_type == POWUP_CONFUSE )
    {
        glm_vec3_copy(
            POWUP_COL_CONFUSE,
            powup_holder->powups[unused].color
        );
        powup_holder->powups[unused].tex_type = TEX_POWUP_CONFUSE;
    }
    else if ( powup_type == POWUP_CHAOS )
    {
        glm_vec3_copy(
            POWUP_COL_CHAOS,
            powup_holder->powups[unused].color
        );
        powup_holder->powups[unused].tex_type = TEX_POWUP_CHAOS;
    }
}

void powup_update( PowupHolder* powup_holder, float dt )
{
    for ( size_t i = 0; i < POWUP_OBJECT_NUM; i++ )
    {
        if ( !powup_holder->powups[i].destroyed )
        {
            vec2 scaled_vel;
            glm_vec2_scale( powup_holder->powups[i].velocity, dt, scaled_vel );
            glm_vec2_add(
                powup_holder->powups[i].position,
                scaled_vel,
                powup_holder->powups[i].position
            );
        }
    }
}

void powup_draw( PowupHolder* powup_holder )
{
    for ( size_t i = 0; i < POWUP_OBJECT_NUM; i++ )
    {
        if ( !powup_holder->powups[i].destroyed )
        {
            renderer_quad_draw(
                powup_holder->powups[i].tex_type,
                powup_holder->powups[i].position,
                powup_holder->powups[i].size,
                powup_holder->powups[i].rotation,
                powup_holder->powups[i].color
            );
        }
    }
}