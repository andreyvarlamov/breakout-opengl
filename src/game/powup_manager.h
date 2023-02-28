#ifndef POWUP_MANAGER_H
#define POWUP_MANAGER_H

#include <stdbool.h>

#include "game_object.h"

#define POWUP_OBJECT_NUM 16

#define POWUP_OBJECT_SIZE ( ( vec2 ) { 60.0f, 20.0f  } )
#define POWUP_OBJECT_VEL  ( ( vec2 ) { 0.0f,  150.0f } )

#define POWUP_COL_SPEED             ( ( vec3 ) { 0.5f, 0.5f, 1.0f } )
#define POWUP_COL_STICKY            ( ( vec3 ) { 1.0f, 0.5f, 1.0f } )
#define POWUP_COL_PASS_THROUGH      ( ( vec3 ) { 0.5f, 1.0f, 0.5f } )
#define POWUP_COL_PAD_SIZE_INCREASE ( ( vec3 ) { 1.0f, 0.6f, 0.4f } )
#define POWUP_COL_CONFUSE           ( ( vec3 ) { 1.0f, 0.3f, 0.3f } )
#define POWUP_COL_CHAOS             ( ( vec3 ) { 0.9f, 0.2f, 0.2f } )

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
    GameObject obj      [POWUP_OBJECT_NUM];
    PowupType  pu_types [POWUP_OBJECT_NUM];
} PowupManager;

// Initialize powup holder
void powup_init( PowupManager* pu_man );

// Respawn a new powerup pickup
void powup_object_respawn(
    PowupManager* pu_man,
    PowupType pu_type,
    vec2 pos
);

// Respawn a new powerup pickup randomly
void powup_object_respawn_random( PowupManager* pu_man, vec2 pos );

// Update power-ups
void powup_update( PowupManager* pu_man, float dt );

// Render power-ups
void powup_object_draw( PowupManager* pu_man );

#endif