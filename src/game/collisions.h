#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "ball_object.h"
#include "game_object.h"

// Check collisions between 2 AABBs
// --------------------------------
bool col_check_rects(
    const GameObject* one,
    const GameObject* two
);

// Check collisions between a circle and an AABB
// ---------------------------------------------
bool col_check_circ_rect(
    const BallObject* circ,
    const GameObject* rect
);

#endif