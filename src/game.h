#ifndef GAME_H
#define GAME_H

#include "gfx/window.h"

typedef enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
} GameState;

typedef struct Game
{
    GameState state;
    Window* window;
} Game;

Game* game_create(Window* window);
void game_destroy(Game** game);

// Initialize game state (load all shaders/textures/levels)
void game_init(Game* game);

// Game loop
void game_process_input(Game* game, float dt);
void game_update(Game* game, float dt);
void game_render();

#endif
