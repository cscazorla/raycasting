#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <stdbool.h>

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event sdl_event;
uint32_t* color_buffer;
SDL_Texture* color_buffer_texture;

int initializeWindow();
void destroyWindow();
void swapBuffer();
void draw_rect(int x, int y, int width, int height, uint32_t color);
void draw_pixel(int x, int y, uint32_t color);
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void draw_mini_map();
void draw_player();
void draw_rays();

#endif