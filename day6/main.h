#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

// used for denoting the direction of the guard as they move though the grid
typedef enum DIRECTION {
    INVALID = 0, // value for if guard is not changing directions
    UP,
    RIGHT,
    DOWN,
    LEFT,
    INF, // for candidacy checks
    EXIT, // value for if guard is at an exit
} DIRECTION;

// used for denoting spaced on the grid
typedef enum TILE {
    EMPTY = '.',
    BLOCKED = '#',
    GUARD = '^',
    TRED = 'X',
    INF_BLOCK = 'O',
} TILE;

/**
 * candidate check
 */
typedef struct candidate {
    uint16_t steps_taken; // holds the steps a guard has taken to help check for inf loops
    uint8_t col; // the current column the space we are checking is on
    uint8_t row; // the current row the current space we are checking is on
    uint8_t guard_init_row; // the initial position of the gurad
    uint8_t guard_init_col; // the initial position of the gurad
    DIRECTION direction; // the direction of the guard was IF they encountered the tile. INVALID otherwise. INF if the guard ended up back here
} candidate;

#endif