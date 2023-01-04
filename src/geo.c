#ifdef WASM
double sqrt(double x);
double pow(double x, double e);
#else
#include <math.h>
#endif

typedef struct {
    float x;
    float y;
}  Vec2;

void normalize(Vec2 *p)
{
    float h = sqrt(pow(p->x, 2) + pow(p->y, 2));
    p->x /= h;
    p->y /= h;
}


void vec2_add(Vec2 *p1, Vec2 *p2)
{
    p1->x += p2->x;
    p1->y += p2->y;
}

void vec2_mul(Vec2 *p1, Vec2 *p2)
{
    p1->x *= p2->x;
    p1->y *= p2->y;
}

float vec2_mag(Vec2 *p)
{
    return sqrt(p->x * p->x + p->y * p->y);
}

void vec2_set_mag(Vec2 *p1, float new_mag)
{
    float multi = vec2_mag(p1) / new_mag;
    p1->x /= multi;
    p1->y /= multi;
}

void vec2_limit(Vec2 *p1, float limit)
{
    if (!(vec2_mag(p1) > limit)) {
        return;
    }
    float multi = vec2_mag(p1) / limit;
    p1->x /= multi;
    p1->y /= multi;
}
