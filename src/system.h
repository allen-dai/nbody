#pragma once

#include "particle.h"

typedef struct {
    int size;
    int capacity;
    Particle **particle;
} System;

System* new_system(int capactiy);
void system_push(System *system, Particle *particle);
int system_remove_at(System *system, int index);
void free_system(System *system);
void system_update(System *system, float G);
void system_reset(System *system);
