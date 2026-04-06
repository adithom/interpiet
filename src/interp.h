#ifndef PIETVM_INTERP_H
#define PIETVM_INTERP_H

#include "types.h"
#include "grid.h"
#include "flood.h"

void interp_run(VM *vm, CodelGrid *grid, BlockMap *bm);

#endif
