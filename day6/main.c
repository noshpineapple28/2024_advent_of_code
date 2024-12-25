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
// candidate for potential inf loop
candidate cand = {0};

// helper methods

/**
 * handles what to do if the guard moves up
 *
 * @param grid the grid we are moving on
 */
void hande_up_move(TILE **grid)
{
    /** DETERMINE NEXT TILE TO GO TO **/
    // if direction above is blocked, set direction right
    if (guard_row >= 1 &&
        (grid[guard_row - 1][guard_col] == BLOCKED ||
         grid[guard_row - 1][guard_col] == INF_BLOCK))
    {
        // update candidacy
        if (cand.direction == INVALID && grid[guard_row - 1][guard_col] == INF_BLOCK)
            cand.direction = guard_direction;
        else if (cand.direction == guard_direction && grid[guard_row - 1][guard_col] == INF_BLOCK)
            cand.direction = INF;

        guard_direction = RIGHT;
        return;
    }

    grid[guard_row][guard_col] = TRED;
    // if space isn't blocked move up
    if (guard_row >= 1)
    {
        guard_row--;
    }
    // if space is the border, set to exit
    else
    {
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
    /** DETERMINE NEXT TILE TO GO TO **/
    // if direction right is blocked, set direction down
    if (guard_col < cols - 1 &&
        (grid[guard_row][guard_col + 1] == BLOCKED ||
         grid[guard_row][guard_col + 1] == INF_BLOCK))
    {
        // set candidacy direction
        if (cand.direction == INVALID && grid[guard_row][guard_col + 1] == INF_BLOCK)
            cand.direction = guard_direction;
        else if (cand.direction == guard_direction && grid[guard_row][guard_col + 1] == INF_BLOCK)
            cand.direction = INF;

        guard_direction = DOWN;
        return;
    }

    grid[guard_row][guard_col] = TRED;
    // if space isn't blocked move right
    if (guard_col < cols - 1)
    {
        guard_col++;
    }
    // if space is the border, set to exit
    else
    {
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
    /** DETERMINE NEXT TILE TO GO TO **/
    // if direction to below is blocked, set direction left
    if (guard_row < rows - 1 &&
        (grid[guard_row + 1][guard_col] == BLOCKED ||
         grid[guard_row + 1][guard_col] == INF_BLOCK))
    {
        // update candidacy
        if (cand.direction == INVALID && grid[guard_row + 1][guard_col] == INF_BLOCK)
            cand.direction = guard_direction;
        else if (cand.direction == guard_direction && grid[guard_row + 1][guard_col] == INF_BLOCK)
            cand.direction = INF;

        guard_direction = LEFT;
        return;
    }

    grid[guard_row][guard_col] = TRED;
    // if space isn't blocked move right
    if (guard_row < rows - 1)
    {
        guard_row++;
    }
    // if space is the border, set to exit
    else
    {
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
    /** DETERMINE NEXT TILE TO GO TO **/
    // if direction to left is blocked, set direction up
    if (guard_col >= 1 &&
        (grid[guard_row][guard_col - 1] == BLOCKED ||
         grid[guard_row][guard_col - 1] == INF_BLOCK))
    {
        // update candidacy
        if (cand.direction == INVALID && grid[guard_row][guard_col - 1] == INF_BLOCK)
            cand.direction = guard_direction;
        else if (cand.direction == guard_direction && grid[guard_row][guard_col - 1] == INF_BLOCK)
            cand.direction = INF;
        guard_direction = UP;
        return;
    }

    grid[guard_row][guard_col] = TRED;
    // if space isn't blocked move left
    if (guard_col >= 1)
    {
        guard_col--;
    }
    // if space is the border, set to exit
    else
    {
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
            tot += (grid[row][col] == TRED);
            col++;
        }
        col = 0;
        row++;
    }

    return tot;
}

/**
 * calculates the total number of columns and rows for a given
 *      grid in a givne file
 * 
 * @param fp the file we are reading from
 */
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

/**
 * resets the grid to an initial position and then moves the potential
 *      infinite loop block up a space
 * 
 * @param grid the grid we are reseting
 * @returns 1 if we have exceeded all possible tiles to check
 *      0 otherwise
 */
uint8_t reset_grid(TILE **grid)
{
    uint8_t col = 0;
    uint8_t row = 0;
    // loop until we find a non block space   
    do
    {
        // change the candidate col
        cand.col++;
        // prevent overreach
        if (cand.col >= cols)
        {
            cand.col = 0;
            cand.row++;
        }

        // exit loop we have checked every tile
        if (cand.row >= rows)
            return 1;    
    } while (grid[cand.row][cand.col] == BLOCKED);
    
    // reset starter vals
    cand.direction = INVALID;
    cand.steps_taken = 0;
    guard_col = cand.guard_init_col;
    guard_row = cand.guard_init_row;
    guard_direction = UP;

    // loop through the grid and reset vals
    while (row < rows)
    {
        while (col < cols)
        {
            // set the inf block
            if (row == cand.row && col == cand.col)
            {
                grid[row][col] = INF_BLOCK;
            }
            // set the guard init pos
            else if (row == cand.guard_init_row && col == cand.guard_init_col)
                grid[row][col] = GUARD;
            // set empty
            else if (grid[row][col] != BLOCKED)
                grid[row][col] = EMPTY;

            // incriment
            col++;
        }
        col = 0;
        row++;
    }

    return 0;
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
                // save on candidate
                cand.guard_init_col = guard_col;
                cand.guard_init_row = guard_row;
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

/**
 * initiates a procedurly generated grid generated from a given file
 * 
 * @param fp the file to generate from
 * @returns pointer to dynamically generated grid
 */
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
    if (grid[0][0] != BLOCKED)
        grid[0][0] = INF_BLOCK;

    return grid;
}

/**
 * runs advent of code day 6
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
    uint16_t tot_inf_loop_blocks = 0;
    printf("TOTAL POSSIBLE INF LOOP TILES: %4d", tot_inf_loop_blocks);

    // loop until the reset func states that it's checked every tile
    do
    {
        while (guard_direction != EXIT && cand.steps_taken < 16900)
        {
            move_guard(grid);
            // if the guard has ended up in the same position we have an inf loop
            if (cand.direction == INF)
            {
                // animation to alert user as to what's going on
                tot_inf_loop_blocks++;
                printf("\b\b\b\b%4d", tot_inf_loop_blocks);
                break;
            }
            // increment steps taken in case we reach an inf loop some other way
            cand.steps_taken++;
        }

        // incriment counter for if inf loop reached without candidacy check
        if (cand.steps_taken >= 16900)
        {
            // animation to alert user as to what's going on
            tot_inf_loop_blocks++;
            printf("\b\b\b\b%4d", tot_inf_loop_blocks);
        }
    } while (!reset_grid(grid));

    // for part 1
    // find total number trodden tiles
    // uint16_t tot = find_tot_tiles_trodden(grid);
    // printf("TOTAL TILES TRODDEN: %d\n", tot);

    // free grid
    uint8_t row = 0;
    while (row < rows)
        free(grid[row++]);
    free(grid);
    // close file!
    fclose(fp);
}
