#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// global vars
static uint16_t file_len = 0;

// helper methods

uint8_t is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

/**
 * loads a files contents to buffer
 *
 * @param fp the file to read from
 * @param c the buffer to copy onto
 * @param len the length of the file
 */
void load_file_to_buffer(FILE *fp, char *c, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
        *(c++) = fgetc(fp);

    // null terminate
    *(c) = 0;
}

/**
 * helper method that determines if a given substring is the start
 *      of a given command
 *
 * @param px substring to check
 * @returns 1 if valid, 0 if invalid
 */
uint8_t custom_str_cmp(char *px, char *cmp)
{
    uint8_t i = 0;
    while (cmp[i] != 0)
    {
        if (*(px + i) != cmp[i])
            return 0;
        i++;
    }
    return 1;
}

/**
 * takes a buffer and a start place and converts a substring of it
 *      to an int
 *
 * @param buffer the buffer to take a substring from
 * @param num_start the position to start taking a substring from
 * @param num_len the position at which the substring ends (exclusive)
 *
 * @returns the converted substring number
 */
uint16_t parse_number(char *buffer, uint16_t num_start, uint16_t num_len)
{
    // we need to make a copy as we need a substring
    char *copy = (char *)malloc((sizeof(char) * (num_len - num_start)) + 1);
    // counter var
    uint8_t i = 0;
    // copy the substring
    while (i < num_len - num_start)
    {
        copy[i] = buffer[num_start + i];
        i++;
    }
    // null terminate
    copy[i] = 0;

    // convert the string to a int
    uint16_t val = atoi(copy);
    // free the malloc
    free(copy);
    // return value
    return val;
}

// main methods

/**
 * initializes a buffer containing the file contents
 *
 * @returns pointer to a malloced buffer with file contents inside
 */
char *init_file()
{
    // open the input file
    FILE *fp;
    fopen_s(&fp, "./input", "r");

    // get file size and allocate
    fseek(fp, 0, SEEK_END);
    file_len = ftell(fp);
    // create a buffer to hold file contents
    char *buffer = (char *)malloc((sizeof(char) * file_len) + 1);
    // go back to beginning
    fseek(fp, 0, SEEK_SET);
    // copy file
    load_file_to_buffer(fp, buffer, file_len);

    // close file
    fclose(fp);

    return buffer;
}

/**
 * passes through a buffer until it finds a `mul(` command, and returns
 *      the index in which it occured
 *
 * @param buffer the buffer to read from
 * @param pos the position to read
 *
 * @returns the position at which the mul occurs
 */
uint16_t seek_mul(char *buffer, uint16_t pos)
{
    uint16_t i = pos;

    while (!custom_str_cmp(buffer + i, "mul(") && *(buffer + i + 3) != 0)
        i++;

    return i;
}

/**
 * seeks through a buffer and seeks how long a number given is
 * it can seek to see how the number is terminated and make sure
 *      its terminated properly
 *
 * @param c pointer to the substring we are checking
 * @param pos the position to start checking at
 * @param terminator the character to check if the string was terminated by
 *
 * @returns 0 if this is an invalid number OR a positive number if
 *       valid at position of the terminator
 */
uint16_t seek_number_end(char *c, uint16_t pos, char terminator)
{
    uint16_t i = pos;

    while (is_digit(*(c + i)))
        i++;

    // if the number is not terminated by the correct terminator
    //      OR the number's length is 0
    // return 0
    if (*(c + i) != terminator || i == pos)
        return 0;

    return i;
}

/**
 * runs through a buffer and seeks out all mul commands,
 *      and returns their added up sums of outputs
 *
 * @param buffer the buffer to read through
 *
 * @returns a total of all the sums of muls in the given file
 */
uint32_t find_muls(char *buffer)
{
    uint16_t pos = 0;
    // vals to help parse muls
    uint16_t num_end = 0;
    uint16_t num1 = 0;
    uint16_t num2 = 0;
    // total product
    uint32_t total = 0;
    // state
    uint8_t do_mul = 1;

    do
    {
        // check for do
        if (!do_mul && custom_str_cmp(buffer + pos, "do()"))
        {
            // pass over onto the next command
            pos += 4;
            do_mul = !do_mul;
            continue;
        }
        // check for mul
        else if (do_mul && custom_str_cmp(buffer + pos, "mul("))
        {
            // holds position at start of string
            uint16_t pos_start = pos;
            // store where in buffer the number ends
            num_end = seek_number_end(buffer, pos + 4, ',');
            if (!num_end)
            {
                pos = pos_start + 1;
                continue;
            }
            num1 = parse_number(buffer, pos + 4, num_end);
            // set position to after first number
            pos = num_end + 1;
            // get where the next number ends
            num_end = seek_number_end(buffer, pos, ')');
            if (!num_end)
            {
                pos = pos_start + 1;
                continue;
            }
            num2 = parse_number(buffer, pos, num_end);
            pos = num_end;
            total += num1 * num2;
        }
        // check for dont
        else if (do_mul && custom_str_cmp(buffer + pos, "don't()"))
        {
            // move to next item
            pos += 7;
            do_mul = !do_mul;
            continue;
        }
        // otherwise go to next item
        else
        {
            pos++;
        }
    } while (buffer[pos + 4] != 0);

    return total;
}

/**
 * runs advent of code day 3
 */
void main()
{
    // create buffer of file contents
    char *buffer = init_file();
    uint32_t total = find_muls(buffer);
    printf("TOTAL PRODUCT: %d", total);

    free(buffer);
}