#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "main.h"

// global vars

// dimensions
uint8_t rows = 0;
uint8_t cols = 0;
// guard direction
DIRECTION guard_direction = UP;
uint8_t guard_row = 0;
uint8_t guard_col = 0;

// helper methods

/**
 * handles what to do if the guard moves up
 *
 * @param grid the grid we are moving on
 */
void hande_up_move(TILE **grid)
{
    // if direction above is blocked, set direction right
    if (guard_row >= 1 && grid[guard_row - 1][guard_col] == BLOCKED)
    {
        guard_direction = RIGHT;
    }
    // if space isn't blocked move up
    else if (guard_row >= 1)
    {
        grid[guard_row][guard_col] = TRODDEN;
        guard_row--;
    }
    // if space is the border, set to exit
    else
    {
        grid[guard_row][guard_col] = TRODDEN;
        guard_direction = EXIT;
    }
}

/**
 * handles what to do if the guard moves right
 *
 * @param grid the grid we are moving on
 */
void hande_right_move(TILE **grid)
{
    // if direction right is blocked, set direction down
    if (guard_col < cols - 1 && grid[guard_row][guard_col + 1] == BLOCKED)
    {
        guard_direction = DOWN;
    }
    // if space isn't blocked move right
    else if (guard_col < cols - 1)
    {
        grid[guard_row][guard_col] = TRODDEN;
        guard_col++;
    }
    // if space is the border, set to exit
    else
    {
        grid[guard_row][guard_col] = TRODDEN;
        guard_direction = EXIT;
    }
}

/**
 * handles what to do if the guard moves down
 *
 * @param grid the grid we are moving on
 */
void hande_down_move(TILE **grid)
{
    // if direction to below is blocked, set direction left
    if (guard_row < rows - 1 && grid[guard_row + 1][guard_col] == BLOCKED)
    {
        guard_direction = LEFT;
    }
    // if space isn't blocked move right
    else if (guard_row < rows - 1)
    {
        grid[guard_row][guard_col] = TRODDEN;
        guard_row++;
    }
    // if space is the border, set to exit
    else
    {
        grid[guard_row][guard_col] = TRODDEN;
        guard_direction = EXIT;
    }
}

/**
 * handles what to do if the guard moves left
 *
 * @param grid the grid we are moving on
 */
void hande_left_move(TILE **grid)
{
    // if direction to left is blocked, set direction up
    if (guard_col >= 1 && grid[guard_row][guard_col - 1] == BLOCKED)
    {
        guard_direction = UP;
    }
    // if space isn't blocked move left
    else if (guard_col >= 1)
    {
        grid[guard_row][guard_col] = TRODDEN;
        guard_col--;
    }
    // if space is the border, set to exit
    else
    {
        grid[guard_row][guard_col] = TRODDEN;
        guard_direction = EXIT;
    }
}

/**
 * goes through a given TILE grid and calculates the total number
 *      of trodden tiles a guard has trod
 *
 * @param grid the grid we are reading
 * @returns the total number of trodden tiles
 */
uint16_t find_tot_tiles_trodden(TILE **grid)
{
    uint8_t row = 0;
    uint8_t col = 0;
    uint16_t tot = 0;
    while (row < rows)
    {
        while (col < cols)
        {
            tot += (grid[row][col] == TRODDEN);
            col++;
        }
        col = 0;
        row++;
    }

    return tot;
}

void calc_tot_cols_rows(FILE *fp)
{
    char c = 0;
    do
    {
        c = fgetc(fp);
        if (c == '\n')
            rows++;
        // only calc cols for first row
        else if (c != '\n' && !rows)
            cols++;
    } while (c != EOF);

    // move file back to start
    fseek(fp, 0, SEEK_SET);
}

// main methods

/**
 * reads through the file and fills the grid of TILES using given data
 *
 * @param fp the file we are reading from
 * @param grid the grid of tiles we are filling
 * @returns a pointer to the updated grid
 */
TILE **fill_grid(FILE *fp, TILE **grid)
{
    uint8_t row = 0;
    uint8_t col = 0;
    TILE t = EMPTY;
    while (row < rows)
    {
        while (col < cols)
        {
            // convert to tile
            t = (TILE)fgetc(fp);
            grid[row][col] = t;
            col++;
            // if this is the guard, set its position
            if (t == GUARD)
            {
                guard_row = row;
                guard_col = col - 1;
            }
        }
        // skip newline
        fgetc(fp);
        col = 0;
        row++;
    }

    return grid;
}

/**
 * moves the guard or changes their direction based on information and
 *      states generated from the grid
 */
void move_guard(TILE **grid)
{
    switch (guard_direction)
    {
    case UP:
        hande_up_move(grid);
        break;
    case RIGHT:
        hande_right_move(grid);
        break;
    case DOWN:
        hande_down_move(grid);
        break;
    case LEFT:
        hande_left_move(grid);
        break;
    default:
        break;
    }
}

TILE **initiate_grid(FILE *fp)
{
    TILE **grid;
    // find tot cols and rows
    calc_tot_cols_rows(fp);

    // malloc the everything
    uint8_t row = 0;
    grid = (TILE **)malloc(sizeof(TILE *) * rows);
    while (row < rows)
        grid[row++] = (TILE *)malloc(sizeof(TILE) * cols);

    // fille grid with tiles
    grid = fill_grid(fp, grid);

    return grid;
}

/**
 * runs advent of code day 5
 */
void main()
{
    // open file
    FILE *fp;
    fp = fopen("./input", "r");

    // the grid we are working with, each tile on it is made up of
    //      a TILE enum
    TILE **grid;
    // initiate tile grid
    grid = initiate_grid(fp);

    // move the guard
    while (guard_direction != EXIT)
        move_guard(grid);
    // find total number trodden tiles
    uint16_t tot = find_tot_tiles_trodden(grid);
    printf("TOTAL TILES TRODDEN: %d", tot);

    // free grid
    uint8_t row = 0;
    while (row < rows)
        free(grid[row++]);
    free(grid);
    // close file!
    fclose(fp);
}