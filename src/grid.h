#ifndef PIETVM_GRID_H
#define PIETVM_GRID_H

#include "types.h"
#include "ppm.h"

CodelGrid   *grid_build(PPMImage *img, int codel_size);
void         grid_free(CodelGrid *grid);
PietColor    classify_color(uint8_t r, uint8_t g, uint8_t b);
void grid_free(CodelGrid *grid);

static inline int grid_in_bounds(CodelGrid *g, int x, int y) {
    return x >= 0 && x < g->width && y >= 0 && y < g->height;
}

static inline PietColor grid_get(CodelGrid *g, int x, int y) {
    return g->cells[y * g->width + x];
}

#endif
