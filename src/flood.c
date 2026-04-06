// bfs to flood fill and detect codels
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "types.h"
#include "grid.h"
#include "flood.h"

void blockmap_free(BlockMap *bm) {
    if (!bm) return;
    for (int i = 0; i < bm->num_blocks; i++)
        free(bm->blocks[i].codels);
    free(bm->blocks);
    free(bm->codel_to_block);
    free(bm);
}

static void flood_fill_into(CodelGrid *grid, BlockMap *bm, int start_x, int start_y, int id) {
    PietColor target = grid_get(grid, start_x, start_y);
    int total        = grid->width * grid->height;

    // BFS queue
    Point *queue = malloc(total * sizeof(Point));
    int head = 0, tail = 0;

    // initialize the block
    ColorBlock *block = &bm->blocks[id];
    block->color  = target;
    block->size   = 0;
    block->codels = malloc(total * sizeof(Point));

    // seed the queue
    bm->codel_to_block[start_y * grid->width + start_x] = id;
    queue[tail++] = (Point){start_x, start_y};

    int dx[] = { 1, -1,  0, 0 };
    int dy[] = { 0,  0,  1, -1 };

    while (head < tail) {
        Point cur = queue[head++];
        block->codels[block->size++] = cur;

        for (int d = 0; d < 4; d++) {
            int nx = cur.x + dx[d];
            int ny = cur.y + dy[d];

            if (!grid_in_bounds(grid, nx, ny)) continue;
            if (bm->codel_to_block[ny * grid->width + nx] != -1) continue;
            if (grid_get(grid, nx, ny) != target) continue;

            bm->codel_to_block[ny * grid->width + nx] = id;
            queue[tail++] = (Point){nx, ny};        }
    }

    block->codels = realloc(block->codels, block->size * sizeof(Point));
    free(queue);
}


BlockMap *blockmap_build(CodelGrid *grid) {
    int total = grid->width * grid->height;

    BlockMap *bm = malloc(sizeof(BlockMap));
    bm->codel_to_block = malloc(total * sizeof(int));
    bm->blocks         = malloc(total * sizeof(ColorBlock));
    bm->num_blocks     = 0;

    // -1 means unvisited
    for (int i = 0; i < total; i++) bm->codel_to_block[i] = -1;

    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            if (bm->codel_to_block[y * grid->width + x] != -1) continue;

            int id = bm->num_blocks++;
            flood_fill_into(grid, bm, x, y, id);
        }
    }

    bm->blocks = realloc(bm->blocks, bm->num_blocks * sizeof(ColorBlock));
    return bm;
}

static int dp_value(Point p, Direction dp) {
    switch (dp) {
        case DP_RIGHT: return  p.x;
        case DP_LEFT:  return -p.x;  // negate so max always means "most extreme"
        case DP_DOWN:  return  p.y;
        case DP_UP:    return -p.y;
    }
}

static int cc_value(Point p, Direction dp, CodelChooser cc) {
    switch (dp) {
        case DP_RIGHT: return cc == CC_LEFT  ? -p.y :  p.y;
        case DP_LEFT:  return cc == CC_LEFT  ?  p.y : -p.y;
        case DP_DOWN:  return cc == CC_LEFT  ?  p.x : -p.x;
        case DP_UP:    return cc == CC_LEFT  ? -p.x :  p.x;
    }
}

Point find_exit_codel(ColorBlock *block, Direction dp, CodelChooser cc) {
    int extreme = INT_MIN;
    for (int i = 0; i < block->size; i++) {
        int val = dp_value(block->codels[i], dp);
        if (val > extreme) extreme = val;
    }

    // collect all codels on that edge
    Point *candidates = malloc(block->size * sizeof(Point));
    int num_candidates = 0;
    for (int i = 0; i < block->size; i++) {
        if (dp_value(block->codels[i], dp) == extreme)
            candidates[num_candidates++] = block->codels[i];
    }

    // stage 2 — pick by CC
    Point result = candidates[0];
    int best = cc_value(candidates[0], dp, cc);

    for (int i = 1; i < num_candidates; i++) {
        int val = cc_value(candidates[i], dp, cc);
        if (val > best) {
            best = val;
            result = candidates[i];
        }
    }

    free(candidates);
    return result;
}
