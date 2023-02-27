#ifndef RENDERER_H
#define RENDERER_H

#include <cglm/cglm.h>

#include "tex_type.h"

/******************************************************************************
 *                     FRAMEBUFFER AND POST PROCESSING                        *
 ******************************************************************************/

// Initialize the framebuffer
void renderer_framebuffer_init();

// Bind the framebuffer; consequent draws will be drawn to this framebuffer
void renderer_framebuffer_bind();

void renderer_framebuffer_unbind();

// Initialize vao, shader, etc. to draw the final framebuffer
void renderer_framebuffer_draw_init();

// Draw the final framebuffer texture after applying post-processing
void renderer_framebuffer_draw();

/******************************************************************************
 *                               QUAD RENDERING                               *
 ******************************************************************************/

// Prepare quad VAO and shader data
void renderer_quad_init( unsigned int game_width, unsigned int game_height );

void renderer_quad_draw(
    TexType tex_type,
    vec2 position,
    vec2 size,
    float rotate,
    vec3 color
);

/******************************************************************************
 *                             PARTICLE RENDERING                             *
 ******************************************************************************/

// Prepare particle VAO and shader data
void renderer_particle_init(
    unsigned int game_width,
    unsigned int game_height
);

/* The idea behind splitting into 3 actions is that there will always be
 * many particles being drawn, so no need to set and unset OpenGL state before
 * and after drawing each particle.
 * But still want to separate the particulars of Particle logic calculation
 * (e.g. life and position), and the particulars of rendering in OpenGL. */

// Bind particle shader, texture, vao
void renderer_particle_draw_prepare( TexType tex_type );

// Draw each particle separately
void renderer_particle_draw_do( vec2 position, vec4 color );

// Unbind particle shader, texture, vao
void renderer_particle_draw_end();

#endif