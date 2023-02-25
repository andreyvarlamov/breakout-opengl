#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "game_level.h"
#include "game_object.h"
#include "ball_object.h"

#define GAME_LEVEL_NUM 4

typedef enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
} GameState;

typedef struct Game
{
    GameState state;
    unsigned int width;
    unsigned int height;
    bool keys[1024];
    GameLevel game_levels[GAME_LEVEL_NUM];
    size_t current_level;
    GameObject player;
    BallObject ball;
} Game;

// Initialize game state (load all shaders/textures/levels)
// --------------------------------------------------------
void game_init( Game* game, unsigned int width, unsigned int height );

// Game loop
// ---------
void game_process_input( Game* game, float dt );
void game_update( Game* game, float dt );
void game_render( Game* game );
void game_clean( Game* game );

#endif
