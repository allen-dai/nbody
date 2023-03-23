#include "walloc.c"
#include "particle.h"
#include "geo.h"


int Particle_Info[8];

Particle* test()
{
    Particle *particle = (Particle *) malloc(sizeof(Particle *));
    return particle;
}

int *get_info(Particle *particle)
{
    Particle_Info[0] = particle->pos.x;
    Particle_Info[1] = particle->pos.y;
    Particle_Info[2] = particle->vel.x;
    Particle_Info[3] = particle->vel.y;
    Particle_Info[4] = particle->acc.x;
    Particle_Info[5] = particle->acc.y;
    Particle_Info[6] = particle->mass;
    Particle_Info[7] = particle->radius;

    return Particle_Info;
}
