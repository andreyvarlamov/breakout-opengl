#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>

#include "gfx.h"

struct Window
{
    GLFWwindow *handle;
    unsigned int width;
    unsigned int height;
    bool keys[1024];
};

void window_create();
void window_loop();

#endif
