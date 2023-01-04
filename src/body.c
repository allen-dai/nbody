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
} Body;

Body *new_body(float x, float y, float v_x, float v_y, float mass)
{
    Body *body    = (Body *) malloc(sizeof(Body));

    body->pos.x   = x;
    body->pos.y   = y;
    body->vel.x   = v_x;
    body->vel.y   = v_y;
    body->acc.x   = 0;
    body->acc.y   = 0;
    body->mass    = mass;
    body->radius  = sqrt(mass) * 2;

    return body;
}


void update_body(Body *body)
{
    vec2_add(&body->vel, &body->acc);
    vec2_add(&body->pos, &body->vel);
    // limit 15j
    if (vec2_mag(&body->vel) > 30) {
        vec2_set_mag(&body->vel, 30);
    }
    body->acc.x = 0;
    body->acc.y = 0;
}

void apply_force(Body *body, Vec2 *force)
{
    force->x /= body->mass;
    force->y /= body->mass;
    if (force->x != force->x) {
        force->x = 0;
    }
    if (force->y != force->y) {
        force->y = 0;
    }
    vec2_add(&body->acc, force);
}

void attract_body(Body *b1, Body *b2, float G)
{
    Vec2 vector = {b1->pos.x - b2->pos.x, b1->pos.y - b2->pos.y};
    float distance = vector.x * vector.x + vector.y * vector.y;
    if (distance < 10000) distance = 10000.0;
    if (distance > 100000) distance = 100000.0;
    float strength = (G * (b1->mass * b2->mass)) / distance;
    vec2_set_mag(&vector, strength);
    apply_force(b2, &vector);
}

void free_body(Body *body)
{
    free(body);
    return;
}
