#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "constants.h"
#include "display.h"

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
            break;
        }
    }
}

void update(float dt) {
    // To Do
}

void render(float dt) {
    draw_map();
    swapBuffer();
}

int main(int argc, char *argv[]) {
    bool isGameRunning = initializeWindow();
    int ticksLastFrame = 0;
    int timeToWait = 0;
    float dt = 0;

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
