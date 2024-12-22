#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

typedef enum DIRECTION {
    INVALID = 0, // value for if guard is not changing directions
    UP,
    RIGHT,
    DOWN,
    LEFT,
    EXIT, // value for if guard is at an exit
} DIRECTION;

typedef enum TILE {
    EMPTY = '.',
    BLOCKED = '#',
    GUARD = '^',
    TRODDEN = 'X',
} TILE;

#endif