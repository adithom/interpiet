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
    COLOR_LIGHT_RED,
    COLOR_RED,
    COLOR_DARK_RED,
    COLOR_LIGHT_YELLOW,
    COLOR_YELLOW,
    COLOR_DARK_YELLOW,
    COLOR_LIGHT_GREEN,
    COLOR_GREEN,
    COLOR_DARK_GREEN,
    COLOR_LIGHT_CYAN,
    COLOR_CYAN,
    COLOR_DARK_CYAN,
    COLOR_LIGHT_BLUE,
    COLOR_BLUE,
    COLOR_DARK_BLUE,
    COLOR_LIGHT_MAGENTA,
    COLOR_MAGENTA,
    COLOR_DARK_MAGENTA,
    COLOR_WHITE,
    COLOR_BLACK,
    COLOR_UNKNOWN
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
