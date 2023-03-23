#include <stdlib.h>
#include <stdbool.h>
#include "particle.h"
#include "geo.h"
#include "math.h"

typedef struct {
    int size;
    int capacity;
    Particle **particle;
} System;

System* new_system(int capactiy)
{
    System *system = (System*) malloc(sizeof(System));
    system->size = 0;
    system->capacity = capactiy;
    system->particle = (Particle **) malloc(capactiy * sizeof(Particle *));
    return system;
}

void system_push(System *system, Particle *particle)
{
    if (system->size+1 >= system->capacity) {
        system->capacity *= 2;
        system->particle = (Particle **) realloc(system->particle, system->capacity * sizeof(Particle *));
    }

    system->particle[system->size] = particle;
    system->size += 1;
}

int system_remove_at(System *system, int index)
{
    if (index >= system->size || system->particle[index] == NULL ) {
        return -1;
    };
    free_particle(system->particle[index]);
    for (int i = index; i < system->size; i++) {
        system->particle[i] = system->particle[i+1];
    }
    system->particle[system->size] = NULL;
    system->size -= 1;
    return 0;
}

void free_system(System *system)
{
    for (int i = 0; i < system->size; i++) {
        free_particle(system->particle[i]);
    }
    free(system);
}

void system_update(System *system, float G)
{
    for (int i = 0; i < system->size; i++) {
        Particle *b1 = system->particle[i];
        for (int j = 0; j < system->size; j++) {
            Particle *b2 = system->particle[j];
            if (i == j) {
                continue;
            }
            attract_particle(b1, b2, G);
        }
    }

    for (int i = 0; i < system->size; i++) {
        update_particle(system->particle[i]);
    }

}

void system_reset(System *system)
{
    for (int i = 0; i < system->size; i++) {
        free_particle(system->particle[i]);
    }
    system -> size = 0;
}
