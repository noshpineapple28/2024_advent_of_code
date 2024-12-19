#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// global vars
static uint16_t rows = 0;
static uint16_t cols = 0;

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
 * @param c the buffer to copy onto
 * @param len the length of the file
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
 * runs advent of code day 4
 */
void main()
{
    // create buffer of file contents
    char **grid = init_file();

    
    // free the grid cols
    uint16_t i = 0;
    while (i < cols)
        free(grid[i++]);
    free(grid);
}