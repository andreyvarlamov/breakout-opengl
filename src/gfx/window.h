#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>

#include "../game.h"
#include "gfx.h"

typedef struct Window
{
    GLFWwindow* handle;
    Game* game;
    unsigned int width;
    unsigned int height;
    bool keys[1024];
} Window;

Window* window_create();
void window_loop(Window* window);
void window_destroy(Window** window);

#endif
