#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#define BRICKS_INIT_CAP 256

#include "game_object.h"

typedef struct GameLevel
{
    GameObject* bricks;
    size_t bricks_tot;
    size_t bricks_cap;
} GameLevel;

// Load level from file
void game_level_load(
    GameLevel* game_level,
    const char* file,
    unsigned int level_width,
    unsigned int level_height
);

// Render level
void game_level_draw( GameLevel* game_level );

// Clean up game level data
void game_level_clean( GameLevel* game_level );

#endif
