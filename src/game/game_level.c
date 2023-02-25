#include "game_level.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "game_object.h"
#include "../gfx/renderer.h"
#include "../util/file_io.h"

void _init_bricks( GameLevel* game_level )
{
    game_level->bricks = ( GameObject* )
        calloc( 1, BRICKS_INIT_CAP * sizeof( GameObject ) );
    game_level->bricks_tot = 0;
    game_level->bricks_cap = BRICKS_INIT_CAP;
}

void _add_brick( GameLevel* game_level, GameObject game_object )
{
    // Double the capacity of allocated memory if ran out of previously
    // allocated memory
    // ----------------------------------------------------------------
    if ( game_level->bricks_tot >= game_level->bricks_cap )
    {
        game_level->bricks = ( GameObject* ) realloc(
            game_level->bricks,
            game_level->bricks_cap * 2 * sizeof( GameObject )
        );
        game_level->bricks_cap = game_level->bricks_cap * 2;
    }

    memcpy(
        &game_level->bricks[game_level->bricks_tot],
        &game_object,
        sizeof( GameObject )
    );

    ++( game_level->bricks_tot );
}

void _clean_bricks( GameLevel* game_level )
{
    free( game_level->bricks );
    game_level->bricks_tot = 0;
    game_level->bricks_cap = 0;
}

unsigned int* _read_from_file(
    const char* file,
    size_t* tile_num_out,
    size_t* row_size_out
)
{
    char* file_content = read_file( file );
    size_t char_num = strlen( file_content );
    size_t tile_num = 0;
    for ( size_t char_iter = 0; char_iter < char_num; ++char_iter )
    {
        if ( file_content[char_iter] != '\n') 
        {
            ++tile_num;
        }
    }

    unsigned int* tile_types = calloc( 1, tile_num * sizeof( unsigned int ) );

    size_t row_size = 0;
    bool is_first_row = true;
    size_t tile_iter = 0;
    for ( size_t char_iter = 0; char_iter < char_num; ++char_iter )
    {
        if ( file_content[char_iter] != '\n' )
        {
            int num = file_content[char_iter] - '0';
            tile_types[tile_iter] = num;
            ++tile_iter;

            if ( is_first_row )
            {
                ++row_size;
            }
        }
        else if ( is_first_row )
        {
            is_first_row = false;
        }
    }

    *tile_num_out = tile_num;
    *row_size_out = row_size;
    return tile_types;
}

void _init_level(
    GameLevel* game_level,
    unsigned int* tile_types,
    size_t tile_num,
    size_t row_size,
    unsigned int px_width,
    unsigned int px_height
)
{
    unsigned int width = row_size;
    unsigned int height = tile_num / row_size;
    float brick_px_width = px_width / ( float ) width;
    float brick_px_height = px_height / ( float ) height;

    _clean_bricks( game_level );
    _init_bricks( game_level );

    for ( size_t i = 0; i < tile_num; ++i )
    {
        unsigned int x = i % width;
        unsigned int y = i / width;

        if ( tile_types[i] == 1 ) // solid
        {
            GameObject brick = game_object_create(
                ( vec2 ) { brick_px_width * x, brick_px_height * y },
                ( vec2 ) { brick_px_width, brick_px_height },
                ( vec2 ) { 0.0f, 0.0f },
                ( vec3 ) { 0.8f, 0.8f, 0.7f },
                TEX_BLOCK_SOLID
            );
            brick.is_solid = true;

            _add_brick( game_level, brick );
        }
        else if ( tile_types[i] > 1 ) // colored brick
        {
            vec3 color;
            glm_vec3_one( color ); // original: white 
            if ( tile_types[i] == 2 )
            {
                glm_vec3_copy( ( vec3 ) { 0.2f, 0.6f, 1.0f }, color );
            }
            else if ( tile_types[i] == 3 )
            {
                glm_vec3_copy( ( vec3 ) { 0.0f, 0.7f, 0.0f }, color );
            }
            else if ( tile_types[i] == 4 )
            {
                glm_vec3_copy( ( vec3 ) { 0.8f, 0.8f, 0.4f }, color );
            }
            else if ( tile_types[i] == 5 )
            {
                glm_vec3_copy( ( vec3 ) { 1.0f, 0.5f, 0.0f }, color );
            }

            GameObject brick = game_object_create(
                ( vec2 ) { brick_px_width * x, brick_px_height * y },
                ( vec2 ) { brick_px_width, brick_px_height },
                ( vec2 ) { 0.0f, 0.0f },
                color,
                TEX_BLOCK
            );

            _add_brick( game_level, brick );
        }
    }
}

void game_level_load(
    GameLevel* game_level,
    const char* file,
    unsigned int px_width,
    unsigned int px_height
)
{
    size_t tile_num;
    size_t row_size;
    unsigned int* tile_types = _read_from_file( file, &tile_num, &row_size );

    _init_level(
        game_level,
        tile_types,
        tile_num,
        row_size,
        px_width,
        px_height
    );
}

void game_level_draw( GameLevel* game_level )
{
    for ( size_t i = 0; i < game_level->bricks_tot; i++ )
    {
        if ( !game_level->bricks[i].destroyed )
        {
            GameObject brick = game_level->bricks[i];
            sprite_draw(
                brick.tex_type,
                brick.position,
                brick.size,
                brick.rotation,
                brick.color
            );
        }
    }
}

void game_level_clean( GameLevel* game_level )
{
    _clean_bricks( game_level );
}
