#pragma once

typedef struct {
    float x;
    float y;
}  Vec2;

void normalize(Vec2 *p);
void vec2_add(Vec2 *p1, Vec2 *p2);
void vec2_mul(Vec2 *p1, Vec2 *p2);
void vec2_set_mag(Vec2 *p1, float mag);
float vec2_mag(Vec2 *p);
void vec2_limit(Vec2 *p1, float limit);
