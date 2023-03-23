#pragma once

#include "geo.h"

typedef struct {
    Vec2 pos;
    Vec2 vel;
    Vec2 acc;
    float mass;
    float radius;
} Particle;

Particle *new_particle( float x, float y, float v_x, float v_y, float mass);

void update_particle(Particle *particle);
void apply_force(Particle *particle, Vec2 *force);
void attract_particle(Particle *b1, Particle *b2, float G);
void free_particle(Particle *particle);
