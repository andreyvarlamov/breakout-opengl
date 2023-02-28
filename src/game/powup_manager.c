#include "powup_manager.h"

#include <stdbool.h>
#include <stdlib.h>

#include <cglm/cglm.h>

#include "../gfx/renderer.h"

/******************************************************************************
 *                                  GENERAL                                   *
 ******************************************************************************/

void powup_init( PowupManager* pu_man )
{
    for ( size_t i = 0; i < POWUP_OBJECT_NUM; i++ )
    {
        pu_man->obj[i].destroyed = true;
    }

    for ( size_t i = 0; i < POWUP_COUNT; i++ )
    {
        pu_man->pu_effects[i] = 0.0f;
    }
}

void powup_update( PowupManager* pu_man, float dt )
{
    for ( size_t i = 0; i < POWUP_OBJECT_NUM; i++ )
    {
        if ( !pu_man->obj[i].destroyed )
        {
            vec2 scaled_vel;
            glm_vec2_scale( pu_man->obj[i].velocity, dt, scaled_vel );
            glm_vec2_add(
                pu_man->obj[i].position,
                scaled_vel,
                pu_man->obj[i].position
            );
        }
    }

    for ( size_t i = 0; i < POWUP_COUNT; i++ )
    {
        if ( pu_man->pu_effects[i] > 0.0f )
        {
            pu_man->pu_effects[i] -= dt;
            if ( pu_man->pu_effects[i] <= 0.0f )
            {
                pu_man->pu_effects[i] = 0.0f;
            }
        }
    }
}

/******************************************************************************
 *                            POWER UP OBJECTS                                *
 ******************************************************************************/

void powup_object_respawn( PowupManager* pu_man, PowupType pu_type, vec2 pos )
{
    size_t unused = -1;
    for ( size_t i = 0; i < POWUP_OBJECT_NUM; i++ )
    {
        if ( pu_man->obj[i].destroyed )
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

    if ( ( ( unsigned int ) pu_type ) >=  POWUP_COUNT )
    {
        // Invalid powup type, skip
        return;
    }

    pu_man->obj[unused].destroyed = false;
    pu_man->pu_types[unused] = pu_type;
    glm_vec2_copy( pos,               pu_man->obj[unused].position );
    glm_vec2_copy( POWUP_OBJECT_SIZE, pu_man->obj[unused].size );
    glm_vec2_copy( POWUP_OBJECT_VEL,  pu_man->obj[unused].velocity );

    if ( pu_type == POWUP_SPEED )
    {
        glm_vec3_copy(
            POWUP_COL_SPEED,
            pu_man->obj[unused].color
        );
        pu_man->obj[unused].tex_type = TEX_POWUP_SPEED;
    }
    else if ( pu_type == POWUP_STICKY )
    {
        glm_vec3_copy(
            POWUP_COL_STICKY,
            pu_man->obj[unused].color
        );
        pu_man->obj[unused].tex_type = TEX_POWUP_STICKY;
    }
    else if ( pu_type == POWUP_PASS_THROUGH )
    {
        glm_vec3_copy(
            POWUP_COL_PASS_THROUGH,
            pu_man->obj[unused].color
        );
        pu_man->obj[unused].tex_type = TEX_POWUP_PASS_THROUGH;
    }
    else if ( pu_type == POWUP_PAD_SIZE_INCREASE )
    {
        glm_vec3_copy(
            POWUP_COL_PAD_SIZE_INCREASE,
            pu_man->obj[unused].color
        );
        pu_man->obj[unused].tex_type = TEX_POWUP_PAD_SIZE_INCREASE;
    }
    else if ( pu_type == POWUP_CONFUSE )
    {
        glm_vec3_copy(
            POWUP_COL_CONFUSE,
            pu_man->obj[unused].color
        );
        pu_man->obj[unused].tex_type = TEX_POWUP_CONFUSE;
    }
    else if ( pu_type == POWUP_CHAOS )
    {
        glm_vec3_copy(
            POWUP_COL_CHAOS,
            pu_man->obj[unused].color
        );
        pu_man->obj[unused].tex_type = TEX_POWUP_CHAOS;
    }
}

void powup_object_respawn_random( PowupManager* pu_man, vec2 pos )
{
    //unsigned int will_spawn = ( rand() % 15 ) == 0;
    unsigned int will_spawn = ( rand() % 3 ) == 0;
    //unsigned int will_spawn = true;
    if ( will_spawn )
    {
        PowupType pu_type;
        unsigned int good_or_bad = rand() % 4;
        if ( good_or_bad == 0 )
        {
            // 1 in 4 chance to spawn a positive power up

            // Equal chance to spawn any of the types
            unsigned int type = rand() % 4;
            if ( type == 0 )
            {
                pu_type = POWUP_SPEED;
            }
            else if ( type == 1 )
            {
                pu_type = POWUP_STICKY;
            }
            else if ( type == 2 )
            {
                pu_type = POWUP_PASS_THROUGH;
            }
            else
            {
                pu_type = POWUP_PAD_SIZE_INCREASE;
            }
        }
        else
        {
            // 3 in 4 chance to spawn a negative power up
            
            // Equal chance to spawn either of the types
            unsigned int type = rand() % 2;
            if ( type == 0 )
            {
                pu_type = POWUP_CONFUSE;
            }
            else
            {
                pu_type = POWUP_CHAOS;
            }
        }

        powup_object_respawn( pu_man, pu_type, pos );
    }
}

void powup_object_draw( PowupManager* pu_man )
{
    for ( size_t i = 0; i < POWUP_OBJECT_NUM; i++ )
    {
        if ( !pu_man->obj[i].destroyed )
        {
            renderer_quad_draw(
                pu_man->obj[i].tex_type,
                pu_man->obj[i].position,
                pu_man->obj[i].size,
                pu_man->obj[i].rotation,
                pu_man->obj[i].color
            );
        }
    }
}

/******************************************************************************
 *                            POWER UP EFFECTS                                *
 ******************************************************************************/

void powup_effect_set( PowupManager* pu_man, PowupType pu_type )
{
    float dur = 0.0f;

    if ( pu_type == POWUP_SPEED )
    {
        dur = POWUP_EFF_DUR_SPEED;
    }
    else if ( pu_type == POWUP_STICKY )
    {
        dur = POWUP_EFF_DUR_STICKY;
    }
    else if ( pu_type == POWUP_PASS_THROUGH )
    {
        dur = POWUP_EFF_DUR_PASS_THROUGH;
    }
    else if ( pu_type == POWUP_PAD_SIZE_INCREASE )
    {
        dur = POWUP_EFF_DUR_PAD_SIZE_INCREASE;
    }
    else if ( pu_type == POWUP_CONFUSE )
    {
        dur = POWUP_EFF_DUR_CONFUSE;
    }
    else if ( pu_type == POWUP_CONFUSE )
    {
        dur = POWUP_EFF_DUR_CHAOS;
    }

    pu_man->pu_effects[pu_type] = dur;
}

bool powup_effect_get( PowupManager* pu_man, PowupType pu_type )
{
    return pu_man->pu_effects[pu_type] > 0.0f;
}