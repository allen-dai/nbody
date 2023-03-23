#include "geo.h"
#ifdef WASM
double sqrt(double x);
#else
#include <stdlib.h>
#include <math.h>
#endif

typedef struct {
    Vec2 pos;
    Vec2 vel;
    Vec2 acc;
    float mass;
    float radius;
} Particle;

Particle *new_particle(float x, float y, float v_x, float v_y, float mass)
{
    Particle *particle    = (Particle *) malloc(sizeof(Particle));

    particle->pos.x   = x;
    particle->pos.y   = y;
    particle->vel.x   = v_x;
    particle->vel.y   = v_y;
    particle->acc.x   = 0;
    particle->acc.y   = 0;
    particle->mass    = mass;
    particle->radius  = sqrt(mass) * 2;

    return particle;
}


void update_particle(Particle *particle)
{
    vec2_add(&particle->vel, &particle->acc);
    vec2_add(&particle->pos, &particle->vel);
    // limit 15j
    if (vec2_mag(&particle->vel) > 30) {
        vec2_set_mag(&particle->vel, 30);
    }
    particle->acc.x = 0;
    particle->acc.y = 0;
}

void apply_force(Particle *particle, Vec2 *force)
{
    force->x /= particle->mass;
    force->y /= particle->mass;
    if (force->x != force->x) {
        force->x = 0;
    }
    if (force->y != force->y) {
        force->y = 0;
    }
    vec2_add(&particle->acc, force);
}

void attract_particle(Particle *b1, Particle *b2, float G)
{
    Vec2 vector = {b1->pos.x - b2->pos.x, b1->pos.y - b2->pos.y};
    float distance = vector.x * vector.x + vector.y * vector.y;
    if (distance < 10000) distance = 10000.0;
    if (distance > 100000) distance = 100000.0;
    float strength = (G * (b1->mass * b2->mass)) / distance;
    vec2_set_mag(&vector, strength);
    apply_force(b2, &vector);
}

void free_particle(Particle *particle)
{
    free(particle);
    return;
}
