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

typedef struct PowupHolder
{
    GameObject powups      [POWUP_OBJECT_NUM];
    PowupType  powup_types [POWUP_OBJECT_NUM];
} PowupHolder;

// Initialize powup holder (need to set destroyed = true initially)
void powup_init( PowupHolder* powup_holder );

// Respawn a new powerup pickup
void powup_respawn( PowupHolder* powup_holder, PowupType powup_type, vec2 pos );

// Respawn a new powerup pickup randomly
void powup_respawn_random( PowupHolder* powup_holder, vec2 pos );

// Update power-ups
void powup_update( PowupHolder* powup_holder, float dt );

// Render power-ups
void powup_draw( PowupHolder* powup_holder );

#endif