#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game/game.h"
#include "gfx/gfx.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

Game _game;

static void _size_callback( GLFWwindow* window, int width, int height )
{
    glViewport( 0, 0, width, height );
    _game.width = width;
    _game.height = height;
}

static void _key_callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mode
)
{
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
    {
        glfwSetWindowShouldClose( window, true );
    }
    else if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            _game.keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            _game.keys[key] = false;
        }
    }
}

int main( int argc, char *argv[] )
{
    if ( !glfwInit() )
    {
        printf( "Failed to initialize GLFW.\n" );
        exit( -1 );
    }

    // Window init
    // -----------
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_RESIZABLE, false );

    GLFWwindow* window = glfwCreateWindow(
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        "Breakout OpenGL",
        NULL,
        NULL
    );

    glfwMakeContextCurrent( window );

    // GLAD: load all OpenGL function pointers
    // ---------------------------------------
    if ( !gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress ) )
    {
        printf( "Failed to initialize GLAD.\n" );
        exit( -1 );
    }

    // Callbacks
    // ---------
    glfwSetKeyCallback( window, _key_callback );
    glfwSetFramebufferSizeCallback( window, _size_callback );

    // OpenGL Configuration
    // -------------------
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    // TODO: Find out: this turns on alpha blending? (and what that is)
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    // Game data init
    // --------------
    game_init( &_game, SCREEN_WIDTH, SCREEN_HEIGHT );

    // Initialize RNG
    // --------------
    srand( ( unsigned ) time(NULL) );

    // Main Loop
    // ---------
    float delta_time = 0.0f;
    float last_frame = 0.0f;

    while ( !glfwWindowShouldClose( window ) )
    {
        // Calculate delta time
        // --------------------
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        glfwPollEvents();

        // Manage user input
        // -----------------
        game_process_input( &_game, delta_time );

        // Update game state
        // -----------------
        game_update( &_game, delta_time );

        // Render
        // ------
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        game_render( &_game );

        glfwSwapBuffers( window );
    }

    // Free memory
    // -----------
    game_clean( &_game );

    glfwTerminate();
    return 0;
}
