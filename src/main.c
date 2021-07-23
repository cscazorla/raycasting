#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "app.h"
#include "display.h"
#include "player.h"
#include "sprite.h"
#include "ray.h"

// Global game variable
struct Game game;

// Read input on every loop
void readInput() {
    SDL_Event sdl_event;
    SDL_PollEvent(&sdl_event);
    switch (sdl_event.type) {
        case SDL_QUIT: {
            game.isGameRunning = false;
            break;
        }
        case SDL_KEYDOWN: {
            if (sdl_event.key.keysym.sym == SDLK_ESCAPE)
                game.isGameRunning = false;
            if (sdl_event.key.keysym.sym == SDLK_UP)
                setPlayerWalkDirection(PLAYER_WALK_DIRECTION_FRONT);
            if (sdl_event.key.keysym.sym == SDLK_DOWN)
                setPlayerWalkDirection(PLAYER_WALK_DIRECTION_BACK);
            if (sdl_event.key.keysym.sym == SDLK_RIGHT)
                setPlayerTurnDirection(PLAYER_TURN_DIRECTION_RIGHT);
            if (sdl_event.key.keysym.sym == SDLK_LEFT)
                setPlayerTurnDirection(PLAYER_TURN_DIRECTION_LEFT);
            if (sdl_event.key.keysym.sym == SDLK_m)
                game.showMiniMap = !game.showMiniMap;
            break;
        }
        case SDL_KEYUP: {
            if (sdl_event.key.keysym.sym == SDLK_UP)
                setPlayerWalkDirection(0);
            if (sdl_event.key.keysym.sym == SDLK_DOWN)
                setPlayerWalkDirection(0);
            if (sdl_event.key.keysym.sym == SDLK_RIGHT)
                setPlayerTurnDirection(0);
            if (sdl_event.key.keysym.sym == SDLK_LEFT)
                setPlayerTurnDirection(0);
            break;
        }
    }
}

void update(float dt) {
    movePlayer(dt);
    castRays();
}

void render(float dt) {
    clearBuffer();
    drawWallProjection();
    drawSpriteProjection();
    if (game.showMiniMap)
        draw_mini_map();
    swapBuffer();
}

int main(int argc, char *argv[]) {
    game.isGameRunning = initializeWindow();
    int ticksLastFrame = 0;
    int timeToWait = 0;
    float dt = 0;

    initializePlayer();

    while (game.isGameRunning) {
        readInput();

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

    destroyResources();

    return 0;
}
