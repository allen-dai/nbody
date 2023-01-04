#include "walloc.c"
#include "body.h"
#include "geo.h"


int Body_Info[8];

Body* test()
{
    Body *body = (Body *) malloc(sizeof(Body *));
    return body;
}

int *get_info(Body *body)
{
    Body_Info[0] = body->pos.x;
    Body_Info[1] = body->pos.y;
    Body_Info[2] = body->vel.x;
    Body_Info[3] = body->vel.y;
    Body_Info[4] = body->acc.x;
    Body_Info[5] = body->acc.y;
    Body_Info[6] = body->mass;
    Body_Info[7] = body->radius;

    return Body_Info;
}
