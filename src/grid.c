#include "grid.h"
#include "ppm.h"
#include "types.h"
#include <stdlib.h>
#include <stdint.h>

PietColor classify_color(uint8_t r, uint8_t g, uint8_t b) {
    uint32_t rgb = ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
    switch (rgb) {
        // reds
        case 0xFFC0C0: return COLOR_LIGHT_RED;
        case 0xFF0000: return COLOR_RED;
        case 0xC00000: return COLOR_DARK_RED;
        // yellows
        case 0xFFFFC0: return COLOR_LIGHT_YELLOW;
        case 0xFFFF00: return COLOR_YELLOW;
        case 0xC0C000: return COLOR_DARK_YELLOW;
        // greens
        case 0xC0FFC0: return COLOR_LIGHT_GREEN;
        case 0x00FF00: return COLOR_GREEN;
        case 0x00C000: return COLOR_DARK_GREEN;
        // cyans
        case 0xC0FFFF: return COLOR_LIGHT_CYAN;
        case 0x00FFFF: return COLOR_CYAN;
        case 0x00C0C0: return COLOR_DARK_CYAN;
        // blues
        case 0xC0C0FF: return COLOR_LIGHT_BLUE;
        case 0x0000FF: return COLOR_BLUE;
        case 0x0000C0: return COLOR_DARK_BLUE;
        // magentas
        case 0xFFC0FF: return COLOR_LIGHT_MAGENTA;
        case 0xFF00FF: return COLOR_MAGENTA;
        case 0xC000C0: return COLOR_DARK_MAGENTA;
        // special
        case 0xFFFFFF: return COLOR_WHITE;
        case 0x000000: return COLOR_BLACK;
        default:       return COLOR_UNKNOWN;
    }
}

CodelGrid *grid_build(PPMImage *img, int codel_size) {
    CodelGrid *g = malloc(sizeof(CodelGrid));
    if (!g) return NULL;

    g->width = img->width / codel_size; 
    g->height = img->height / codel_size;
    g->cells = malloc(g->width * g->height * sizeof(PietColor));

    if (!g->cells) {
        free(g);
        return NULL;
    }

    for (int cy = 0; cy < g->height; cy++) {
        for (int cx = 0; cx < g->width; cx++) {
            int px = cx * codel_size;
            int py = cy * codel_size;

            uint8_t *pixel = ppm_pixel(img, px, py);
            PietColor color = classify_color(pixel[0], pixel[1], pixel[2]);

            g->cells[cy * g->width + cx] = color;
        }
    }

    return g;
} 
