#include "game.h"

#include <stdio.h>
#include <stdlib.h>

Game* game_create()
{
    Game* game = (Game*) malloc(sizeof(Game*));
    game->state = GAME_ACTIVE;
    return game;
}

void game_destroy(Game** game)
{
    free(*game);
    *game = NULL;
}

void game_init()
{
}

void game_process_input(Game* game, float dt)
{
}

void game_update(Game* game, float dt)
{
}

void game_render(Game* game)
{
}
