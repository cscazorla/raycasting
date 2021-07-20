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
#include "ray.h"
#include "textures.h"

static SDL_Window* window;
static SDL_Renderer* renderer;
static uint32_t* color_buffer;
static SDL_Texture* color_buffer_texture;

/*
 * Function: initializeWindow
 * -------------------
 * Initialize SDL libraries and variables to render things on the screen
 * 
 * returns: true/false if the operation succeeded
 */
bool initializeWindow() {
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
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );

    // Load textures through uPNG
    loadTextures();

    return true;
}

/*
 * Function: destroyWindow
 * -------------------
 * Destroy variables coping with the rendering on the screen
 * 
 * returns: void
 */
void destroyResources() {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        upng_t* upng = textures[i].upngTexture;
        upng_free(upng);
    }
    free(color_buffer);
    SDL_DestroyTexture(color_buffer_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void clearBuffer() {
    for (int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++)
        color_buffer[i] = 0x00000000;
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
 * Function: draw_mini_map
 * -------------------
 * Draws the minimap
 * 
 * returns: void
 */
void draw_mini_map() {
    // Map
    for (int i = 0; i < MAP_NUM_ROWS; i++) {
        for (int j = 0; j < MAP_NUM_COLS; j++) {
            int tileX = j * TILE_SIZE;
            int tileY = i * TILE_SIZE;
            uint32_t tileColor = getMapTileColor(i, j);

            draw_rect(
                (tileX + TILE_SIZE/2.0) * MINIMAP_SCALE_FACTOR, 
                (tileY + TILE_SIZE/2.0) * MINIMAP_SCALE_FACTOR, 
                TILE_SIZE * MINIMAP_SCALE_FACTOR, 
                TILE_SIZE * MINIMAP_SCALE_FACTOR, 
                tileColor
            );
        }
    }

    // Player
    struct Player player = getPlayer();
    draw_rect(
        player.x * MINIMAP_SCALE_FACTOR,
        player.y * MINIMAP_SCALE_FACTOR,
        player.width * MINIMAP_SCALE_FACTOR,
        player.height * MINIMAP_SCALE_FACTOR,
        0xFF0000FF
    );

    // Rays
    for (int i = 0; i < NUM_RAYS; i++) {
        draw_line(
            player.x * MINIMAP_SCALE_FACTOR,
            player.y * MINIMAP_SCALE_FACTOR,
            getRayWallHitX(i) * MINIMAP_SCALE_FACTOR,
            getRayWallHitY(i) * MINIMAP_SCALE_FACTOR,
            0xFF00FFFF
        );
    }
}

/*
 * Function: draw_wall_projection
 * -------------------
 * Draws the 3d projection of the map on the screen
 * 
 * returns: void
 */
void draw_wall_projection() {
    struct Player player = getPlayer();
    for (int i = 0; i < NUM_RAYS; i++) {
        float correctedDistance = getRayWallHitDistance(i) * cos(getRayAngle(i) - player.rotationAngle);
        float projectedWallHeight = ((float)TILE_SIZE / correctedDistance) * DIST_PROJ_PLANE;

        // Get top and bottom pixels
        int wallTopPixel = (WINDOW_HEIGHT / 2) - (projectedWallHeight / 2);
        wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;
        int wallBottomPixel = (WINDOW_HEIGHT / 2) + (projectedWallHeight / 2);
        wallBottomPixel = wallBottomPixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomPixel;

        // Render the ceiling on the color buffer
        for (int j = 0; j < wallTopPixel; j++) {
            draw_pixel(i, j, 0xFF555555);
        }
        
        // Render the wall on the color buffer
        int offsetX = getRayWasHitVertical(i)?(int)getRayWallHitY(i) % TILE_SIZE:(int)getRayWallHitX(i) % TILE_SIZE;
        int textIndex = getRayHitContent(i) - 1;
        float intensityShadingFactor = (float)(200.0) / getRayWallHitDistance(i);
        for (int j = wallTopPixel; j < wallBottomPixel; j++) {
            int topDistance = j + (projectedWallHeight / 2) - (WINDOW_HEIGHT / 2);
            int offsetY = topDistance * ((float)TEXTURE_HEIGHT / projectedWallHeight);
            uint32_t color = textures[textIndex].texture_buffer[(TEXTURE_WIDTH * offsetY) + offsetX];
            changeColorIntensity(&color, intensityShadingFactor);
            draw_pixel(i, j, color);
        }

        // Render the floor on the color buffer
        for (int j = wallBottomPixel; j < WINDOW_HEIGHT; j++) {
            draw_pixel(i, j, 0xFF666666);
        }

    }
}

/*
 * Function: changeColorIntensity
 * -------------------
 * Changes the intensity (bright) of a color by a factor
 * 
 * uint32_t* color: Pointer to the color to be updated
 * float factor: Factor between 0 and 1 to be applied to the color
 * 
 * returns: void
 */
void changeColorIntensity(uint32_t* color, float factor) {
    factor = (factor > 1.0)?1.0:((factor < 0.0)?0.0:factor);
    uint32_t c1 = (*color & 0xFF000000);
    uint32_t c2 = (*color & 0x00FF0000) * factor;
    uint32_t c3 = (*color & 0x0000FF00) * factor;
    uint32_t c4 = (*color & 0x000000FF) * factor;
    *color = c1 | (c2 & 0x00FF0000) | (c3 & 0x0000FF00) | (c4 & 0x000000FF);
}
