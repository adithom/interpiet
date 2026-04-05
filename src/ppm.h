#ifndef PIETVM_PPM_H
#define PIETVM_PPM_H

#include "types.h"

PPMImage *ppm_load(const char *path);
void      ppm_free(PPMImage *img);

static inline uint8_t *ppm_pixel(PPMImage *img, int x, int y) {
    return &img->pixels[(y * img->width + x) * 3];
}

#endif
