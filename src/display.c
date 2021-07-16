#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "constants.h"
#include "display.h"
#include "map.h"
#include "player.h"

/*
 * Function: initializeWindow
 * -------------------
 * Initialize SDL libraries and variables to render things on the screen
 * 
 * returns: true/false if the operation succeeded
 */
int initializeWindow() {
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
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );

    return true;
}

/*
 * Function: destroyWindow
 * -------------------
 * Destroy variables coping with the rendering on the screen
 * 
 * returns: void
 */
void destroyWindow() {
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/*
 * Function: swapBuffer
 * -------------------
 * We use an intermediate array buffer (color_buffer) to render things on the screen.
 * This function does the clearing up, swapping and rendering with SDL
 * 
 * returns: void
 */
void swapBuffer() {
    // Render Color Buffer: Move bits from color_buffer to SDL color_buffer_texture
    SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)(WINDOW_WIDTH * sizeof(uint32_t)));
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
    
    // Clear color_buffer
    memset(color_buffer, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(uint32_t));

    // Swap the video buffer
    SDL_RenderPresent(renderer);
}

/*
 * Function: draw_rect
 * -------------------
 * Draw a rectangle on the screen by setting the center of it, 
 * the width and the height
 * 
 *   <== width ==>
 *   --------------
 *   |            |
 *   |    x,y     | height
 *   |            |
 *   --------------
 * 
 * int x: Horizontal pixel for the center of the rectangle
 * int y: Vertical pixel for the center of the rectangle
 * int width: Rectangle width in pixels
 * int height: Rectangle height in pixels
 * uint32_t color: Color to fill the rectangle
 * 
 * returns: void
 */
void draw_rect(int x, int y, int width, int height, uint32_t color) {
    for (int i = -width/2.0; i < width/2.0; i++) {
        for (int j = -height/2.0; j < height/2.0; j++) {
            int current_x = x + i;
            int current_y = y + j;
            if (current_x >= 0 && current_x < WINDOW_WIDTH && current_y >= 0 && current_y < WINDOW_HEIGHT) {
                color_buffer[(WINDOW_WIDTH * current_y) + current_x] = color;
            }
        }
    }
}

/*
 * Function: draw_pixel
 * -------------------
 * Draw a pixel on the screen 
 * 
 * int x: Horizontal pixel coordinate
 * int y: Vertical pixel coordinate
 * uint32_t color: Color for the pixel
 * 
 * returns: void
 */
void draw_pixel(int x, int y, uint32_t color) {
    if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT) {
        color_buffer[(WINDOW_WIDTH * y) + x] = color;
    }
}

/*
 * Function: draw_line
 * -------------------
 * Draw a line on the screen 
 * 
 * (x0, y0) x==========x (x1, y1)
 * 
 * int x0: Horizontal pixel coordinate for the starting point
 * int y0: Vertical pixel coordinate for the starting point
 * int x1: Horizontal pixel coordinate for the ending point
 * int y1: Vertical pixel coordinate for the ending point
 * uint32_t color: Color for the line
 * 
 * returns: void
 */
void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);

    int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

    float x_inc = delta_x / (float)longest_side_length;
    float y_inc = delta_y / (float)longest_side_length;

    float current_x = x0;
    float current_y = y0;

    for (int i = 0; i <= longest_side_length; i++) {
        draw_pixel(round(current_x), round(current_y), color);
        current_x += x_inc;
        current_y += y_inc;
    }
}

/*
 * Function: draw_map
 * -------------------
 * Draws the minimap
 * 
 * returns: void
 */
void draw_map() {
    for (int i = 0; i < MAP_NUM_ROWS; i++) {
        for (int j = 0; j < MAP_NUM_COLS; j++) {
            int tileX = j * TILE_SIZE;
            int tileY = i * TILE_SIZE;
            uint32_t tileColor = getMapTilecolor(i, j);

            draw_rect(
                (tileX + TILE_SIZE/2.0) * MINIMAP_SCALE_FACTOR, 
                (tileY + TILE_SIZE/2.0) * MINIMAP_SCALE_FACTOR, 
                TILE_SIZE * MINIMAP_SCALE_FACTOR, 
                TILE_SIZE * MINIMAP_SCALE_FACTOR, 
                tileColor
            );
        }
    }
}

/*
 * Function: draw_player
 * -------------------
 * Draws the player on minimap
 * 
 * returns: void
 */
void draw_player() {
    // Body
    draw_rect(
        player.x * MINIMAP_SCALE_FACTOR,
        player.y * MINIMAP_SCALE_FACTOR,
        player.width * MINIMAP_SCALE_FACTOR,
        player.height * MINIMAP_SCALE_FACTOR,
        0xFF00FF00
    );

    // Looking at?
    draw_line(
        player.x * MINIMAP_SCALE_FACTOR,
        player.y * MINIMAP_SCALE_FACTOR,
        (player.x + cos(player.rotationAngle) * 40.0f) * MINIMAP_SCALE_FACTOR,
        (player.y + sin(player.rotationAngle) * 40.0f) * MINIMAP_SCALE_FACTOR,
        0xFFFF0000
    );
}