#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <stdbool.h>

#include <cglm/cglm.h>

#include "ball_object.h"
#include "game_object.h"

typedef enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
} Direction;

typedef struct Collision
{
    bool is;
    Direction dir;
    vec2 diff;
} Collision;

// Check collisions between 2 AABBs
bool col_check_rects( GameObject one, GameObject two );

// Check collisions between a circle and an AABB
Collision col_check_circ_rect( BallObject circ, GameObject rect );

#endif