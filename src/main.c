#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "constants.h"
#include "display.h"
#include "player.h"
#include "ray.h"

// Read input on every loop
void readInput(bool *isGameRunning) {
    SDL_PollEvent(&sdl_event);
    switch (sdl_event.type) {
        case SDL_QUIT: {
            *isGameRunning = false;
            break;
        }
        case SDL_KEYDOWN: {
            if (sdl_event.key.keysym.sym == SDLK_ESCAPE)
                *isGameRunning = false;
            if (sdl_event.key.keysym.sym == SDLK_UP)
                player.walkDirection = 1;
            if (sdl_event.key.keysym.sym == SDLK_DOWN)
                player.walkDirection = -1;
            if (sdl_event.key.keysym.sym == SDLK_RIGHT)
                player.turnDirection = 1;
            if (sdl_event.key.keysym.sym == SDLK_LEFT)
                player.turnDirection = -1;
            break;
        }
        case SDL_KEYUP: {
            if (sdl_event.key.keysym.sym == SDLK_UP)
                player.walkDirection = 0;
            if (sdl_event.key.keysym.sym == SDLK_DOWN)
                player.walkDirection = -0;
            if (sdl_event.key.keysym.sym == SDLK_RIGHT)
                player.turnDirection = 0;
            if (sdl_event.key.keysym.sym == SDLK_LEFT)
                player.turnDirection = -0;
            break;
        }
    }
}

void update(float dt) {
    movePlayer(dt);
    castRays();
}

void render(float dt) {
    draw_mini_map();
    draw_rays();
    draw_player();
    swapBuffer();
}

void start() {
    player.x = WINDOW_WIDTH / 2;
    player.y = WINDOW_HEIGHT / 4;
    player.width = 10;
    player.height = 10;
    player.turnDirection = 0;
    player.walkDirection = 0;
    player.rotationAngle = PI / 4;
    player.walkSpeed = 100;
    player.turnSpeed = 90 * (PI / 180);
}

int main(int argc, char *argv[]) {
    bool isGameRunning = initializeWindow();
    int ticksLastFrame = 0;
    int timeToWait = 0;
    float dt = 0;

    start();

    while (isGameRunning) {
        readInput(&isGameRunning);

        // Calculate delta time
        timeToWait = FRAME_TIME_LENGTH - (SDL_GetTicks() - ticksLastFrame);
        if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH) {
            SDL_Delay(timeToWait);
        }
        dt = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
        ticksLastFrame = SDL_GetTicks();

        update(dt);
        render(dt);
    }

    destroyWindow();

    return 0;
}
