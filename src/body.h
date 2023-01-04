#pragma once

#include "geo.h"

typedef struct {
    Vec2 pos;
    Vec2 vel;
    Vec2 acc;
    float mass;
    float radius;
} Body;

Body *new_body( float x, float y, float v_x, float v_y, float mass);

void update_body(Body *body);
void apply_force(Body *body, Vec2 *force);
void attract_body(Body *b1, Body *b2, float G);
void free_body(Body *body);
