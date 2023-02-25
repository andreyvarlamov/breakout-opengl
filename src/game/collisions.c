#include "collisions.h"

#include <stdbool.h>

#include <cglm/cglm.h>

#include "ball_object.h"
#include "game_object.h"

bool col_check_rects(
    const GameObject* one,
    const GameObject* two
)
{
    // X-axis
    // ------
    bool col_x = (one->position[0] + one->size[0] >= two->position[0])
              && (two->position[0] + two->size[0] >= one->position[0]);

    // Y-axis
    // ------
    bool col_y = (one->position[1] + one->size[1] >= two->position[1])
              && (two->position[1] + two->size[1] >= one->position[1]);

    return col_x && col_y;
}

bool col_check_circ_rect(
    const BallObject* circ,
    const GameObject* rect
)
{
    vec2 circ_center;
    glm_vec2_adds( circ->d.position, circ->radius, circ_center );

    vec2 rect_half_extents;
    glm_vec2_scale( rect->size, 0.5f, rect_half_extents );

    vec2 rect_center;
    glm_vec2_add( rect->position, rect_half_extents, rect_center );

    // Calculate the diff vec between ball center and rect center
    // ----------------------------------------------------------
    vec2 diff;
    glm_vec2_sub( circ_center, rect_center, diff );

    // Clamp the diff vec within the rect half extents
    // -----------------------------------------------
    float clamped_x = glm_clamp(
        diff[0],
        -rect_half_extents[0],
        rect_half_extents[0]
    );
    float clamped_y = glm_clamp(
        diff[1],
        -rect_half_extents[1],
        rect_half_extents[1]
    );
    vec2 clamped = { clamped_x, clamped_y };

    // Find the closest point from the rect to ball
    // --------------------------------------------
    vec2 closest;
    glm_vec2_add( rect_center, clamped, closest );

    // Find the distance between the closest point to rect and ball center
    // -------------------------------------------------------------------
    vec2 diff2;
    glm_vec2_sub( circ_center, closest, diff2 );

    float dist = glm_vec2_distance( GLM_VEC2_ZERO, diff2 );

    // If that distance is less than ball radius -> collision
    // ------------------------------------------------------

    bool collision = dist < circ->radius;
    return collision;
}