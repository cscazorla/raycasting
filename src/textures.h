#ifndef TEXTURES_H
#define TEXTURES_H

// Textures
#define NUM_TEXTURES 4
#define TEXTURE_WIDTH 64
#define TEXTURE_HEIGHT 64

#include <stdint.h>
#include "constants.h"
#include "upng.h"

typedef struct {
    upng_t* upngTexture;
    int width;
    int height;
    uint32_t* texture_buffer;
} texture_t;

texture_t textures[NUM_TEXTURES];

void loadTextures();

#endif