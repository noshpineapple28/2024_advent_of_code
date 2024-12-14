#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// global vars
static FILE *fp;

// helper methods

uint8_t is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

// main methods

/**
 * runs advent of code day 3
 */
void main()
{
    // open the input file
    fopen_s(&fp, "./input", "r");

    // close file
    fclose(fp);
}