#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// global vars
static uint8_t rows = 0;
static uint8_t cols = 0;

// helper methods

/**
 * reads through the file and determines how many
 *      rows and columns there are
 */
void parse_rows_and_cols(FILE *fp)
{
    char c;
    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '\n')
            rows++;
        // only update cols if we are on first row
        else if (!rows)
            cols++;
    }
}

/**
 * loads a files contents to buffer
 *
 * @param fp the file to read from
 * @param grid the grid to save to
 */
void load_file_to_buffer(FILE *fp, char **grid)
{
    for (uint16_t i = 0; i < rows; i++)
    {
        for (uint16_t j = 0; j < cols; j++)
        {
            grid[i][j] = fgetc(fp);
        }
        // skip newline character
        fgetc(fp);
    }
}

/**
 * helper method that determines if a secetion contains xmas
 *
 * @param grid the string to check
 * @returns 1 if valid, 0 if invalid
 */
uint8_t custom_str_cmp(char *px)
{
    char *cmp = "XMAS";
    uint8_t i = 0;
    while (cmp[i] != 0)
    {
        if (*(px + i) != cmp[i])
            return 0;
        i++;
    }
    return 1;
}

// main methods

/**
 * initializes a grid containing the file contents
 *
 * @returns pointer to a malloced grid with file contents inside
 */
char **init_file()
{
    // open the input file
    FILE *fp;
    fopen_s(&fp, "./input", "r");

    // get the total rows and cols
    parse_rows_and_cols(fp);
    fseek(fp, 0, SEEK_SET);

    // outer buffer that holds all the rows
    char **grid = (char **)malloc((sizeof(char *) * rows));

    // malloc the grid with cols
    uint16_t i = 0;
    while (i < cols)
        grid[i++] = (char *)malloc(sizeof(char) * cols);

    // copy file
    load_file_to_buffer(fp, grid);

    // close file
    fclose(fp);

    return grid;
}

/**
 * takes a given position and searches the surrounding 8 cells
 *      directions to see if they make XMAS
 *
 * @param grid the word search grid that we are looking though
 * @param row the row of the grid to look at
 * @param col the column of the grid to look at
 * @returns a number (max 8) of times this cell was the start of XMAS in
 *      one of 8 orthagonal directions
 */
uint8_t check_for_xmas(char **grid, uint8_t row, uint8_t col)
{
    uint8_t matches = 0;
    /**
     * * x x
     * x o x
     * x x x
     */
    if (row >= 3 && col >= 3)
    {
        char val[4] = {grid[row][col], grid[row - 1][col - 1], grid[row - 2][col - 2], grid[row - 3][col - 3]};
        matches += custom_str_cmp(val);
    }
    /**
     * x * x
     * x o x
     * x x x
     */
    if (row >= 3)
    {
        char val[4] = {
            grid[row][col],
            grid[row - 1][col],
            grid[row - 2][col],
            grid[row - 3][col],
        };
        matches += custom_str_cmp(val);
    }
    /**
     * x x *
     * x o x
     * x x x
     */
    if (row >= 3 && col <= cols - 4)
    {
        char val[4] = {grid[row][col], grid[row - 1][col + 1], grid[row - 2][col + 2], grid[row - 3][col + 3]};
        matches += custom_str_cmp(val);
    }
    /**
     * x x x
     * * o x
     * x x x
     */
    if (col >= 3)
    {
        char val[4] = {grid[row][col], grid[row][col - 1], grid[row][col - 2], grid[row][col - 3]};
        matches += custom_str_cmp(val);
    }
    /**
     * x x x
     * x o *
     * x x x
     */
    if (col <= cols - 4)
    {
        char val[4] = {grid[row][col], grid[row][col + 1], grid[row][col + 2], grid[row][col + 3]};
        matches += custom_str_cmp(val);
    }
    /**
     * x x x
     * x o x
     * * x x
     */
    if (row <= rows - 4 && col >= 3)
    {
        char val[4] = {grid[row][col], grid[row + 1][col - 1], grid[row + 2][col - 2], grid[row + 3][col - 3]};
        matches += custom_str_cmp(val);
    }
    /**
     * x x x
     * x o x
     * x * x
     */
    if (row <= rows - 4)
    {
        char val[4] = {grid[row][col], grid[row + 1][col], grid[row + 2][col], grid[row + 3][col]};
        matches += custom_str_cmp(val);
    }
    /**
     * x x x
     * x o x
     * x x *
     */
    if (row <= rows - 4 && col <= cols - 4)
    {
        char val[4] = {grid[row][col], grid[row + 1][col + 1], grid[row + 2][col + 2], grid[row + 3][col + 3]};
        matches += custom_str_cmp(val);
    }

    return matches;
}

/**
 * searches through grid for appearances of xmas
 *
 * @param grid the word search grid to look through
 * @returns the number of counterd xmas's
 */
uint16_t search_grid(char **grid)
{
    uint8_t i = 0;
    uint8_t j = 0;
    uint16_t xmas_cnt = 0;
    while (i < rows)
    {
        while (j < cols)
        {
            xmas_cnt += check_for_xmas(grid, i, j);
            j++;
        }
        // increment counters and recent j
        j = 0;
        i++;
    }

    return xmas_cnt;
}

/**
 * runs advent of code day 4
 */
void main()
{
    // create buffer of file contents
    char **grid = init_file();

    uint16_t cnt = search_grid(grid);
    printf("COUNT: %d", cnt);

    // free the grid cols
    uint16_t i = 0;
    while (i < cols)
        free(grid[i++]);
    free(grid);
}

// 2257 too low
