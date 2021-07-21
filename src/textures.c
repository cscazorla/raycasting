#include <stdio.h>
#include "textures.h"

static const char* textureFileNames[NUM_TEXTURES] = {
    "./assets/wall-stone.png",
    "./assets/brick-grey.png",
    "./assets/brick-red.png",
    "./assets/metallic-door.png",
    "./assets/armor.png",
    "./assets/barrel.png",
    "./assets/bones.png",
    "./assets/hanged.png",
};

/*
 * Function: loadTextures
 * -------------------
 * Load texture files from the disk to the textures array
 * 
 * returns: void
 */
void loadTextures() {
    upng_t* upng;
    for (int i = 0; i < NUM_TEXTURES; i++) {
        upng = upng_new_from_file(textureFileNames[i]);
        if (upng != NULL) {
            upng_decode(upng);
            if (upng_get_error(upng) == UPNG_EOK) {
                textures[i] = upng;
            } else {
                printf("Error decoding texture file %s \n", textureFileNames[i]);
            }
        } else {
            printf("Error loading texture file %s \n", textureFileNames[i]);
        }
    }
}

/*
 * Function: freeTextures
 * -------------------
 * Free upng resources
 * 
 * returns: void
 */
void freeTextures() {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        upng_free(textures[i]);
    }
}