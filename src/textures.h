#ifndef TEXTURES_H
#define TEXTURES_H
#include <stdint.h>
#include "constants.h"
#include "upng.h"

// Textures
#define NUM_TEXTURES 8
#define TEXTURE_WIDTH 64
#define TEXTURE_HEIGHT 64

upng_t* textures[NUM_TEXTURES];

void loadTextures();
void freeTextures();

#endif