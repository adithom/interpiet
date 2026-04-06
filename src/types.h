#ifndef INTERPIET_TYPES_H
#define INTERPIET_TYPES_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// the direction pointer
typedef enum {
  DP_RIGHT = 0,
  DP_DOWN = 1,
  DP_LEFT = 2,
  DP_UP = 3
} Direction;

// chooser for when the interpreter hits a wall with multiple exits
typedef enum {
    CC_LEFT  = 0,
    CC_RIGHT = 1
} CodelChooser;

typedef enum {
    COLOR_LIGHT_RED     = 0,
    COLOR_RED           = 1,
    COLOR_DARK_RED      = 2,
    COLOR_LIGHT_YELLOW  = 3,
    COLOR_YELLOW        = 4,
    COLOR_DARK_YELLOW   = 5,
    COLOR_LIGHT_GREEN   = 6,
    COLOR_GREEN         = 7,
    COLOR_DARK_GREEN    = 8,
    COLOR_LIGHT_CYAN    = 9,
    COLOR_CYAN          = 10,
    COLOR_DARK_CYAN     = 11,
    COLOR_LIGHT_BLUE    = 12,
    COLOR_BLUE          = 13,
    COLOR_DARK_BLUE     = 14,
    COLOR_LIGHT_MAGENTA = 15,
    COLOR_MAGENTA       = 16,
    COLOR_DARK_MAGENTA  = 17,
    COLOR_WHITE         = 18,
    COLOR_BLACK         = 19,
    COLOR_UNKNOWN       = 20
} PietColor;

typedef struct {
    int x;
    int y;
} Point;

// converted from ppm, raw array with the pixel colors
typedef struct {
    int width;
    int height;
    uint8_t *pixels;
} PPMImage;

// stripped of the pixel nature of the earlier struct and assigned a predefined color
typedef struct {
    int width;
    int height;
    PietColor *cells;
} CodelGrid;

// contains all the info about a specific color block, no matter the shape
typedef struct {
    PietColor color;
    int size;
    Point *codels;
} ColorBlock;

// lookup table for all the colorblocks in the grid
typedef struct {
    ColorBlock *blocks;
    int num_blocks;
    int *codel_to_block;
} BlockMap;

// a stackkk
typedef struct {
    int *data;
    int top;
    int capacity;
} Stack;

// vm state
typedef struct {
    Stack stack;
    Direction dp;
    CodelChooser cc;
    int x;
    int y;
} VM;

#endif // !INTERPIET_TYPES_H
