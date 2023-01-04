#include <stdlib.h>
#include <stdbool.h>
#include "body.h"
#include "geo.h"
#include "math.h"

typedef struct {
    int size;
    int capacity;
    Body **body;
} System;

System* new_system(int capactiy)
{
    System *system = (System*) malloc(sizeof(System));
    system->size = 0;
    system->capacity = capactiy;
    system->body = (Body **) malloc(capactiy * sizeof(Body *));
    return system;
}

void system_push(System *system, Body *body)
{
    if (system->size+1 >= system->capacity) {
        system->capacity *= 2;
        system->body = (Body **) realloc(system->body, system->capacity * sizeof(Body *));
    }

    system->body[system->size] = body;
    system->size += 1;
}

int system_remove_at(System *system, int index)
{
    if (index >= system->size || system->body[index] == NULL ) {
        return -1;
    };
    free_body(system->body[index]);
    for (int i = index; i < system->size; i++) {
        system->body[i] = system->body[i+1];
    }
    system->body[system->size] = NULL;
    system->size -= 1;
    return 0;
}

void free_system(System *system)
{
    for (int i = 0; i < system->size; i++) {
        free_body(system->body[i]);
    }
    free(system);
}

void system_update(System *system, float G)
{
    for (int i = 0; i < system->size; i++) {
        Body *b1 = system->body[i];
        for (int j = 0; j < system->size; j++) {
            Body *b2 = system->body[j];
            if (i == j) {
                continue;
            }
            attract_body(b1, b2, G);
        }
    }

    for (int i = 0; i < system->size; i++) {
        update_body(system->body[i]);
    }

}

void system_reset(System *system)
{
    for (int i = 0; i < system->size; i++) {
        free_body(system->body[i]);
    }
    system -> size = 0;
}
