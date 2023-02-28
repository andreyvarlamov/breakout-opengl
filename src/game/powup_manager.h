#ifndef POWUP_MANAGER_H
#define POWUP_MANAGER_H

#include <stdbool.h>

#include "game_object.h"

/******************************************************************************
 *                                  DEFINES                                   *
 ******************************************************************************/

#define POWUP_OBJECT_NUM 16

#define POWUP_OBJECT_SIZE ( ( vec2 ) { 60.0f, 20.0f  } )
#define POWUP_OBJECT_VEL  ( ( vec2 ) { 0.0f,  250.0f } )

#define POWUP_COL_SPEED             ( ( vec3 ) { 0.5f, 0.5f, 1.0f } )
#define POWUP_COL_STICKY            ( ( vec3 ) { 1.0f, 0.5f, 1.0f } )
#define POWUP_COL_PASS_THROUGH      ( ( vec3 ) { 0.5f, 1.0f, 0.5f } )
#define POWUP_COL_PAD_SIZE_INCREASE ( ( vec3 ) { 1.0f, 0.6f, 0.4f } )
#define POWUP_COL_CONFUSE           ( ( vec3 ) { 1.0f, 0.3f, 0.3f } )
#define POWUP_COL_CHAOS             ( ( vec3 ) { 0.9f, 0.2f, 0.2f } )

#define POWUP_EFF_DUR_SPEED             0.0f
#define POWUP_EFF_DUR_STICKY            10.0f
#define POWUP_EFF_DUR_PASS_THROUGH      5.0f
#define POWUP_EFF_DUR_PAD_SIZE_INCREASE 8.0f
#define POWUP_EFF_DUR_CONFUSE           4.0f
#define POWUP_EFF_DUR_CHAOS             4.0f

/******************************************************************************
 *                                   TYPES                                    *
 ******************************************************************************/

typedef enum PowupType
{
    POWUP_SPEED,
    POWUP_STICKY,
    POWUP_PASS_THROUGH,
    POWUP_PAD_SIZE_INCREASE,
    POWUP_CONFUSE,
    POWUP_CHAOS,
    POWUP_COUNT
} PowupType;

typedef struct PowupManager
{
    GameObject obj        [POWUP_OBJECT_NUM];
    PowupType  pu_types   [POWUP_OBJECT_NUM];

    float      pu_effects [POWUP_COUNT];
    bool       to_disable [POWUP_COUNT];
} PowupManager;

/******************************************************************************
 *                                  GENERAL                                   *
 ******************************************************************************/

// Initialize powup holder
void powup_init( PowupManager* pu_man );

// Update power-ups
void powup_update( PowupManager* pu_man, float dt );

/******************************************************************************
 *                            POWER UP OBJECTS                                *
 ******************************************************************************/

// Respawn a new powerup pickup
void powup_object_respawn(
    PowupManager* pu_man,
    PowupType pu_type,
    vec2 pos
);

// Respawn a new powerup pickup randomly
void powup_object_respawn_random( PowupManager* pu_man, vec2 pos );

// Render power-ups
void powup_object_draw( PowupManager* pu_man );

/******************************************************************************
 *                            POWER UP EFFECTS                                *
 ******************************************************************************/

void powup_effect_set( PowupManager* pu_man, PowupType pu_type );

bool powup_effect_get_to_disable( PowupManager* pu_man, PowupType pu_type );

#endif