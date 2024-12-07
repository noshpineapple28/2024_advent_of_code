#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// global vars
static FILE *fp;

// dynamically allocated lists
static uint32_t *left;
static uint32_t *right;

// answers values!
uint32_t distance = 0;
uint32_t similarity = 0;

/**
 * reads through the file and determines how many lines there is
 *
 * @returns number of lines
 */
uint32_t get_num_lines()
{
    uint32_t lines = 0;
    char c;
    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '\n')
            lines++;
    }

    return lines;
}

/**
 * reads through the file and converts string numbers
 *      to uint32_t's
 */
void parse_file()
{
    // buffer to hold current number we are reading, all nums are 5 digits
    char buf[6];
    // index in buffer
    uint8_t i;
    // holds which list to put new num onto
    uint8_t is_right = 0;
    // holds cur_char
    char c;

    // left array index
    uint16_t left_i = 0;
    // right array index
    uint16_t right_i = 0;
    while ((c = fgetc(fp)) != EOF)
    {
        // if a space, we know to split
        if (c == ' ' || c == '\n')
        {
            // terminate buf
            buf[i] = 0;

            // save num
            if (is_right)
                right[right_i++] = atoi(buf);
            else
                left[left_i++] = atoi(buf);

            // reset buf
            i = 0;
            // swap list
            is_right = !is_right;
        }

        // perform operations to help splitting
        if (c == '\n')
        {
            // continue to next number without skipping further ahead
            continue;
        }
        else if (c == ' ')
        {
            // skip 2 to start reading next number
            fgetc(fp);
            fgetc(fp);
            continue;
        }

        // add char to buf
        buf[i++] = c;
    }
}

/**
 * sorts a given list from least to greatest order using
 *      bubble sort
 *
 * @param len the length of the list
 * @param list the list to be sorted
 */
void sort_lists(uint16_t len, uint32_t *list)
{
    // holds swapping var
    uint32_t swap;
    // loop thru the len array times to ensure sortedness
    for (int i = 0; i < len; i++)
    {
        // sorts array
        for (int j = 0; j < len - i - 1; j++)
        {
            // swap if values are out of order
            if (list[j] > list[j + 1])
            {
                // save the greater value
                swap = list[j];
                // swap the two
                list[j] = list[j + 1];
                list[j + 1] = swap;
            }
        }
    }
}

/**
 * computes total distance and similarities between left and right arrays
 */
void compute_total_distance_and_similarity(uint16_t len)
{
    for (uint16_t i; i < len; i++)
    {
        // doin this instead of math.abs cus the arrays are uints
        distance += (left[i] > right[i]) ? left[i] - right[i] : right[i] - left[i];
        // for this index on the left array, loop through and count occurrences
        //      of this exact num in the right array
        // once left[i] is < right, we can break
        for (uint16_t j = 0; left[i] >= right[j]; j++)
        {
            // add left[i] to simularity if equal
            if (left[i] == right[j])
                similarity += left[i];
        }
    }
}

/**
 * runs advent of code day 1
 */
void main()
{
    // open the input file
    fopen_s(&fp, "./input", "r");

    // get total lines
    uint32_t list_size = get_num_lines();
    // reset file pntr
    fseek(fp, 0, SEEK_SET);

    // allocate memory for each list
    left = (uint32_t *)malloc(sizeof(uint32_t) * list_size);
    right = (uint32_t *)malloc(sizeof(uint32_t) * list_size);

    // fill arrays
    parse_file();

    // sort arrays
    sort_lists(list_size, left);
    sort_lists(list_size, right);

    // compare distances
    compute_total_distance_and_similarity(list_size);
    printf("distance: %d\n", distance);
    printf("similarity: %d\n", similarity);

    // free memory
    free(left);
    free(right);
}