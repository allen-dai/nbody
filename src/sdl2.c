#include <SDL2/SDL.h>sdl
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "body.h"
#include "system.h"
#include "geo.h"

#define WIDTH           1920
#define HEIGHT          1080
#define Gravity         20
#define VELOCITY_LIMIT  1
#define Step            0.05

void draw_circle(SDL_Renderer *renderer, int x, int y, int radius);
void fill_circle(SDL_Renderer *renderer, int x, int y, int radius);

int main()
{
    float zoom = 3;
    float mass = 1;
    float G = Gravity;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize the SDL2 library\n");
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                                          SDL_WINDOW_SHOWN);

    if (!window) {
        printf("Failed to create window\n");
        return -1;
    }

    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        return -1;
    }
    SDL_RenderSetLogicalSize(renderer, zoom * WIDTH, zoom * HEIGHT);

    Body *mouse_circle = new_body(0, 0, 0, 0, 20);
    bool running = true;
    int mouse_x, mouse_y;
    SDL_Event e;
    System *system = new_system(1);
    int count = 0;
    bool mousedown = false;

    Vec2 new_body_vel = {0, 0};
    for (int z = 0; z < 1000; z++ ){
        int x = rand() % (WIDTH+800) + (WIDTH - 400);
        int y = rand() % (HEIGHT+400) + (HEIGHT - 200);
        Body *new = new_body(x, y, 0, 0, 1);
        system_push(system, new);
    }
    while (running) {
        uint64_t start = SDL_GetPerformanceCounter();

        SDL_GetMouseState(&mouse_x, &mouse_y);
        mouse_x *= zoom;
        mouse_y *= zoom;
        if (!mousedown) {
            mouse_circle->pos.x = (float)mouse_x;
            mouse_circle->pos.y = (float)mouse_y;
        }

        SDL_PollEvent(&e);
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
            else if (e.key.keysym.sym == SDLK_r) {
                system_reset(system);
                for (int z = 0; z < 1000; z++ ){
                    int x = rand() % (WIDTH+800) + (WIDTH - 400);
                    int y = rand() % (HEIGHT+400) + (HEIGHT - 200);
                    Body *new = new_body(x, y, 0, 0, 1);
                    system_push(system, new);
                }
            }
            else if (e.key.keysym.sym == SDLK_a) {
                mass += 50;
            }
            else if (e.key.keysym.sym == SDLK_d) {
                if (mass >= 50) mass -= 50;
            }
            else if (e.key.keysym.sym == SDLK_n) {
                G += 10;
            }
            else if (e.key.keysym.sym == SDLK_m) {
                G -= 10;
            }
            else if (e.key.keysym.sym == SDLK_l) {
                for (int z = 0; z < 1000; z++ ){
                    int x = rand() % (WIDTH+800) + (WIDTH - 400);
                    int y = rand() % (HEIGHT+400) + (HEIGHT - 200);
                    Body *new = new_body(x, y, 0, 0, 1);
                    system_push(system, new);
                }
            }
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            switch (e.button.button) {
                case SDL_BUTTON_LEFT:
                    mousedown = true;
                    break;
                defaut:
                    break;
            }
        }
        else if (e.type == SDL_MOUSEBUTTONUP) {
            Body *new = new_body(mouse_circle->pos.x, mouse_circle->pos.y, new_body_vel.x, new_body_vel.y, mass);
            system_push(system, new);
            mousedown = false;
        }
        else if (e.type == SDL_MOUSEWHEEL) {
            if(e.wheel.y > 0) {
                zoom -= Step;
            }
            else if(e.wheel.y < 0) {
                zoom += Step;
            }
            SDL_RenderSetLogicalSize(renderer, zoom * WIDTH, zoom * HEIGHT);
        }

        system_update(system, G);
        // Initialize renderer color white for the background
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        // Clear screen
        SDL_RenderClear(renderer);
        // Set renderer color red to draw the square
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        // Draw mouse ball
        if (mousedown) {
            SDL_RenderDrawLine(renderer, mouse_x, mouse_y, mouse_circle->pos.x, mouse_circle->pos.y);
            float x = -((float)mouse_x - (float)mouse_circle->pos.x);
            float y = -((float)mouse_y - (float)mouse_circle->pos.y);
            float h = sqrt(x * x + y * y);
            new_body_vel.x = x;
            new_body_vel.y = y;
            if (x!=0 && y!=0){
                normalize(&new_body_vel);
                vec2_set_mag(&new_body_vel, h);
                vec2_limit(&new_body_vel, VELOCITY_LIMIT);
            }
        }
        draw_circle(renderer, mouse_circle->pos.x, mouse_circle->pos.y, mouse_circle->radius * zoom);
        for (int i = 0; i < system->size; i++) {
            fill_circle(renderer, system->body[i]->pos.x, system->body[i]->pos.y, system->body[i]->radius);
        }
        // Update screen
        SDL_RenderPresent(renderer);

        // Capping at 60fps
        uint64_t end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        // printf("%f\n", 1000.0f / elapsedMS);
        // SDL_Delay(floor(16.66f - elapsedMS));
    }


    free_system(system);
    free_body(mouse_circle);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void draw_circle(SDL_Renderer *renderer, int x0, int y0, int radius)
{
    int x = radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y) {
        SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
        SDL_RenderDrawPoint(renderer, x0 + y, y0 + x);
        SDL_RenderDrawPoint(renderer, x0 - y, y0 + x);
        SDL_RenderDrawPoint(renderer, x0 - x, y0 + y);
        SDL_RenderDrawPoint(renderer, x0 - x, y0 - y);
        SDL_RenderDrawPoint(renderer, x0 - y, y0 - x);
        SDL_RenderDrawPoint(renderer, x0 + y, y0 - x);
        SDL_RenderDrawPoint(renderer, x0 + x, y0 - y);
        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }

        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void fill_circle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;
    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}
