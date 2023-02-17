#include "window.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void _size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void _key_callback(
        GLFWwindow *window,
        int key,
        int scancode,
        int action,
        int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            // TODO
        }
        else if (action == GLFW_RELEASE)
        {

        }
    }
}

void window_create()
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

    GLFWwindow* window = glfwCreateWindow(
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            "Breakout OpenGL",
            NULL,
            NULL);

    if (window == NULL)
    {
        printf("Failed to create a GLFW window.\n");
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);

    // GLAD: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD.\n");
        exit(-1);
    }

    // Callbacks
    // ---------
    glfwSetKeyCallback(window, _key_callback);
    glfwSetFramebufferSizeCallback(window, _size_callback);

    // OpenGL Configuration
    // -------------------
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    // TODO: Find out: this turns on alpha blending? (and what that is)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void window_loop()
{
    // TODO
    while (1)
    {
    }
}
