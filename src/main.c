#include <stdio.h>
#include <stdlib.h>

#include "gfx/window.h"

int main(int argc, char *argv[])
{
    Window* window = window_create();
    window_loop(window);
    window_destroy(&window);
}
