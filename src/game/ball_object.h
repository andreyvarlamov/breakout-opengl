#ifndef BALL_OBJECT_H
#define BALL_OBJECT_H

#include <stdbool.h>

#include "game_object.h"

typedef struct BallObject
{
    GameObject d;
    float radius;
    bool stuck;
    bool sticky;
    bool pass_through;
} BallObject;

BallObject ball_object_create(
    vec2 pos,
    vec2 velocity,
    float radius
);

void ball_object_move(
    BallObject* ball,
    float dt,
    unsigned int window_width
);

#endif