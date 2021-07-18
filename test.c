#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event sdl_event;
uint32_t* color_buffer;
SDL_Texture* color_buffer_texture;

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

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );
    
    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    // Allocate the required memory in bytes to hold the color buffer
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
    
    // Creating a SDL texture that is used to display the color buffer
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );

    bool isGameRunning = true;

    while (isGameRunning) {
        readInput(&isGameRunning);

        int x = 100;
        int y = 200;
        uint32_t color = 0xFF000000;
        color_buffer[(WINDOW_WIDTH * y) + x] = color;
        
        // Render Color Buffer: Move bits from color_buffer to SDL color_buffer_texture
        SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)(WINDOW_WIDTH * sizeof(uint32_t)));
        SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
        
        // Clear color_buffer
        memset(color_buffer, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(uint32_t));

        // Swap the video buffer
        SDL_RenderPresent(renderer);
    }

    free(color_buffer);
    SDL_DestroyTexture(color_buffer_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
