#pragma once

#include "body.h"

typedef struct {
    int size;
    int capacity;
    Body **body;
} System;

System* new_system(int capactiy);
void system_push(System *system, Body *body);
int system_remove_at(System *system, int index);
void free_system(System *system);
void system_update(System *system, float G);
void system_reset(System *system);
