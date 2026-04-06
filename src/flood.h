#ifndef PIETVM_FLOOD_H
#define PIETVM_FLOOD_H

#include "types.h"
#include "grid.h"

BlockMap   *blockmap_build(CodelGrid *grid);
void        blockmap_free(BlockMap *bm);
Point       find_exit_codel(ColorBlock *block, Direction dp, CodelChooser cc);

static inline ColorBlock *blockmap_get(BlockMap *bm, CodelGrid *g, int x, int y) {
    int id = bm->codel_to_block[y * g->width + x];
    return &bm->blocks[id];
}

#endif
