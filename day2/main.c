#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <Math.h>

// global vars
static FILE *fp;

/**
 * given an array checks to see if the numbers within
 *      aren't the same, are no more than 3 apart, and
 *      if they're properly ascending/descending
 * 
 * @param px pointer to the array of numbers we are checking
 * @param len the length of the array
 * 
 * @returns 1 if safe, 0 if not
 */
uint8_t determine_list_safety(uint8_t *px, uint8_t len)
{
    uint8_t i = 1;
    uint8_t is_asc = px[0] < px[1];
    while (i < len)
    {
        int8_t dif = abs(px[i] - px[i - 1]);
        if (
            (is_asc && px[i] < px[i - 1]) ||
            (!is_asc && px[i] > px[i - 1]) ||
            (dif < 1 || dif > 3))
        {
            return 0;
        }

        i++;
    }

    return 1;
}

/**
 * runs through a line to check if it is safe
 * 
 * @param px pointer to the array of numbers we are checking
 * @param len the length of the array
 * 
 * @returns 1 if safe, 0 if not
 */
uint8_t check_line(uint8_t *px, uint8_t len)
{
    uint8_t i = 0;
    uint8_t *copy = (uint8_t *)malloc(sizeof(uint8_t) * len - 1);

    // check if base list is fine
    if (determine_list_safety(px, len))
        return 1;

    while (i < len)
    {
        // copy list excluding current index
        for (int j = 0; j < len; j++)
            copy[j] = px[j];
        for (int j = i + 1; j < len; j++)
            copy[j - 1] = px[j];
            
        // check this current list
        if (determine_list_safety(copy, len - 1))
            return 1;

        i++;
    }

    // free the line
    free(copy);

    return 0;
}

/**
 * parses a line of numbers into an array of integers
 *
 * @returns 0 if unsafe, 1 if safe, 2 if EOF found
 */
uint8_t read_line()
{
    // holds cur character and cur parsed num
    char buf[3];
    char nums[8];
    uint8_t buf_i = 0;
    uint8_t nums_read = 0;
    char c;
    // return value
    uint8_t is_safe = 1;

    do
    {
        // get character
        c = fgetc(fp);

        // determine whether to parse or save char
        if (c == ' ' || c == '\n')
        {
            // null terminate
            buf[buf_i] = 0;
            buf_i = 0;
            // update counts
            nums[nums_read++] = atoi(buf);
        }
        else if (c == EOF)
        {
            // return note that its EOF
            return 2;
        }
        else
        {
            // add to array
            buf[buf_i++] = c;
            continue;
        }

    } while (c != EOF && c != '\n');

    return check_line(nums, nums_read);
}

/**
 * runs advent of code day 2
 */
void main()
{
    // open the input file
    fopen_s(&fp, "./input", "r");

    uint16_t total_safe = 0;
    uint8_t output = 0;

    do
    {
        output = read_line();
        total_safe += (output == 2) ? 0 : output;
    } while (output != 2);

    printf("Total Safe: %d\n", total_safe);

    // close file
    fclose(fp);
}