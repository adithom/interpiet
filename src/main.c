#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "ppm.h"
#include "grid.h"
#include "flood.h"
#include "stack.h"
#include "colors.h"
#include "ops.h"
#include "interp.h"

int main(int argc, char **argv) {
    // parse -v flag
    int verbose    = 0;
    int arg_offset = 1;
    if (argc > 1 && strcmp(argv[1], "-v") == 0) {
        verbose    = 1;
        arg_offset = 2;
    }

    if (argc < arg_offset + 1) {
        fprintf(stderr, "usage: pietvm [-v] <image.ppm> [codel_size]\n");
        return 1;
    }
    const char *path = argv[arg_offset];
    int codel_size   = (argc >= arg_offset + 2) ? atoi(argv[arg_offset + 1]) : 1;

    if (codel_size < 1) {
        fprintf(stderr, "error: codel_size must be >= 1\n");
        return 1;
    }

    // load image
    PPMImage *img = ppm_load(path);
    if (!img) {
        fprintf(stderr, "error: could not load %s\n", path);
        return 1;
    }

    // build codel grid
    CodelGrid *grid = grid_build(img, codel_size);
    ppm_free(img);
    if (!grid) {
        fprintf(stderr, "error: could not build codel grid\n");
        return 1;
    }

    // print grid dimensions and color distribution
    if (verbose) {
        fprintf(stderr, "[debug] grid: %dx%d codels (codel_size=%d)\n",
                grid->width, grid->height, codel_size);
    }
    {
        int counts[21] = {0};
        for (int i = 0; i < grid->width * grid->height; i++)
            counts[(int)grid->cells[i]]++;
        if (verbose) {
            const char *names[] = {
                "light_red","red","dark_red","light_yellow","yellow","dark_yellow",
                "light_green","green","dark_green","light_cyan","cyan","dark_cyan",
                "light_blue","blue","dark_blue","light_magenta","magenta","dark_magenta",
                "white","black","UNKNOWN"
            };
            for (int i = 0; i <= 20; i++) {
                if (counts[i] > 0)
                    fprintf(stderr, "[debug]   %-16s: %d\n", names[i], counts[i]);
            }
        }
        if (counts[20] > 0)
            fprintf(stderr, "warning: %d UNKNOWN codels will act as walls\n", counts[20]);
    }
    if (verbose) {
        const char *sym[] = {
            "LR","R ","DR","LY","Y ","DY",
            "LG","G ","DG","LC","C ","DC",
            "LB","B ","DB","LM","M ","DM",
            "W ","BK","??"
        };
        fprintf(stderr, "[debug] grid layout (x=col, y=row):\n");
        for (int y = 0; y < grid->height; y++) {
            fprintf(stderr, "  row %d: ", y);
            for (int x = 0; x < grid->width; x++)
                fprintf(stderr, "%s ", sym[(int)grid_get(grid, x, y)]);
            fprintf(stderr, "\n");
        }
    }

    // build block map
    BlockMap *bm = blockmap_build(grid);
    if (!bm) {
        fprintf(stderr, "error: could not build block map\n");
        grid_free(grid);
        return 1;
    }
    if (verbose)
        fprintf(stderr, "[debug] blockmap: %d blocks\n", bm->num_blocks);

    // init vm
    VM vm;
    vm.stack = stack_init();
    vm.dp    = DP_RIGHT;
    vm.cc    = CC_LEFT;
    vm.x     = 0;
    vm.y     = 0;

    // run
    interp_run(&vm, grid, bm, verbose);

    // cleanup
    stack_free(&vm.stack);
    blockmap_free(bm);
    grid_free(grid);

    return 0;
}
