#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// global vars
static FILE *fp;

/**
 * continues through a line until the terminator has been found
 */
void read_to_end_of_line()
{
    char c;
    while ((c = fgetc(fp)) != '\n')
        ;
}

/**
 * reads through a line to determine whether its safe
 * 
 * @returns 0 if unsafe, 1 if safe, 2 if EOF found
 */
uint8_t read_line()
{
    // holds cur character and cur parsed num
    char buf[3];
    uint8_t buf_i = 0;
    uint8_t nums_read = 0;
    char c;
    // states
    uint8_t is_asc = 0;
    uint8_t cur_num = 0;
    uint8_t prev_num = 0;
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
            // update counts
            buf_i = 0;
            nums_read++;
            // swap vals and set new num
            prev_num = cur_num;
            cur_num = atoi(buf);
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

        // determine is asc or desc
        if (nums_read == 2)
        {
            is_asc = (prev_num < cur_num);
        }
        /**
         * checks to make sure:
         * 
         * number is staying asc or desc,
         * number is different,
         * number changes by 1 - 3 digits
         */
        if (
            nums_read >= 2 &&
            ((is_asc && (prev_num > cur_num || cur_num - prev_num > 3)) ||
             (!is_asc && (prev_num < cur_num || prev_num - cur_num > 3)) ||
             prev_num == cur_num))
        {
            // if not end of a line, read to the end
            if (c != '\n' && c != EOF)
                read_to_end_of_line();

            return 0;
        }

    } while (c != EOF && c != '\n');

    return 1;
}

/**
 * runs advent of code day 1
 */
void main()
{
    // open the input file
    fopen_s(&fp, "./input", "r");

    uint16_t total_safe = 0;
    uint8_t output = 0;
    uint16_t tot = 1;

    do
    {
        output = read_line();
        total_safe += (output == 2) ? 0 : output;
        tot++;
    } while (output != 2);

    printf("Total Safe: %d\n", total_safe);

    // close file
    fclose(fp);
}