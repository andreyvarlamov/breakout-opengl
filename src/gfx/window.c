#include "window.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../game.h"
#include "gfx.h"

static void _size_callback(GLFWwindow *handle, int width, int height)
{
    glViewport(0, 0, width, height);

    Window* window = (Window*) glfwGetWindowUserPointer(handle);
    if (!window)
    {
        printf("Could not get window user pointer\n");
    }

    window->width = width;
    window->height = height;
}

static void _key_callback(
        GLFWwindow *handle,
        int key,
        int scancode,
        int action,
        int mode)
{
    Window* window = (Window*) glfwGetWindowUserPointer(handle);
    if (!window)
    {
        printf("Could not get window user pointer\n");
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(handle, true);
    }
    else if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            window->keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            window->keys[key] = false;
        }
    }
}

Window* window_create()
{
    if (!glfwInit())
    {
        printf("Failed to initialize GLFW.\n");
        exit(-1);
    }

    // Window init
    // -----------
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    Window* window = (Window*) malloc(sizeof(Window));
    if (window == NULL)
    {
        printf("Failed to allocate memory for window\n");
        exit(-1);
    }

    window->width = 800;
    window->height = 600;

    // Initialize keys array
    for (int i = 0; i < 1024; ++i)
    {
        window->keys[i] = false;
    }

    window->handle = glfwCreateWindow(
            window->width,
            window->height,
            "Breakout OpenGL",
            NULL,
            NULL);


    glfwSetWindowUserPointer(window->handle, window);

    if (window->handle == NULL)
    {
        printf("Failed to create a GLFW window.\n");
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window->handle);

    // GLAD: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD.\n");
        exit(-1);
    }

    // Callbacks
    // ---------
    glfwSetKeyCallback(window->handle, _key_callback);
    glfwSetFramebufferSizeCallback(window->handle, _size_callback);

    // OpenGL Configuration
    // -------------------
    glViewport(0, 0, window->width, window->height);
    // TODO: Find out: this turns on alpha blending? (and what that is)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return window;
}

void window_loop(Window* window)
{
    // TODO
    while (!glfwWindowShouldClose(window->handle))
    {
        glfwPollEvents();
    }
}

void window_destroy(Window** window)
{
    free(*window);
    *window = NULL;
}
