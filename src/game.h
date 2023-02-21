#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

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
} Game;

Game* game_create();
void game_destroy(Game** game);

// Initialize game state (load all shaders/textures/levels)
void game_init(Game* game, unsigned int width, unsigned int height);

// Game loop
void game_process_input(Game* game, float dt);
void game_update(Game* game, float dt);
void game_render();

#endif
