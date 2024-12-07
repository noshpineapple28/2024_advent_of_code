#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// global vars
static FILE *fp; 

uint32_t get_num_lines(FILE **fp)
{
    uint32_t lines = 0;

    return lines;
}

void main()
{
    // open the input file
    fopen_s(&fp, "./input", "r");
    char c = fgetc(fp);
    putchar(c);
    c = fgetc(fp);
    putchar(c);

    fseek(fp, 0, SEEK_SET);
    c = fgetc(fp); 
    putchar(c);
}