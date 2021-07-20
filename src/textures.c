#include <stdio.h>
#include "textures.h"

static const char* textureFileNames[NUM_TEXTURES] = {
    "./assets/wall-stone.png",
    "./assets/brick-grey.png",
    "./assets/brick-red.png",
    "./assets/metallic-door.png"
};

void loadTextures() {
    upng_t* upng;
    for (int i = 0; i < NUM_TEXTURES; i++) {
        upng = upng_new_from_file(textureFileNames[i]);
        if (upng != NULL) {
            upng_decode(upng);
            if (upng_get_error(upng) == UPNG_EOK) {
                textures[i].upngTexture = upng;
                textures[i].width = upng_get_width(upng);
                textures[i].height = upng_get_height(upng);
                textures[i].texture_buffer = (uint32_t*)upng_get_buffer(upng);
            }
        }
    }
}