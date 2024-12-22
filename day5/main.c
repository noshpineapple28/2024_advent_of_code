#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// global vars
// no manual has more than 100 pages (phew)
// every outer array holds the rules for the page
// ex: page_rules[48]
//      the rules for page 48
// ex: page_rules[48][11] == 1
//      page 11 MUST come BEFOR page 48 to be a 1, else it'll be a 0
static uint8_t page_rules[100][100];

// helper methods

/**
 * finds the number of pages in a manual
 *
 * @param fp the file we are reading from
 * @returns the number of pages
 */
uint8_t find_num_pages_in_manual(FILE *fp)
{
    uint8_t total = 0;
    uint8_t chars_read = 0;
    char c = 0;
    do
    {
        // get the char
        fread(&c, sizeof(char), 1, fp);
        chars_read++;
        // if a comma or newline, it means we found a new item
        if (c == ',' || c == '\n')
            total++;
    } while (c != '\n' && c != EOF && c != 0);

    // move it on back now yall
    // (1 hop this time) - repeat for x amaount of characters
    fseek(fp, -1 * chars_read, SEEK_CUR);
    return total;
}

// main methods

/**
 * a fun little bubble sort that swaps values if a page should
 *      come after the given value
 * 
 * @param manual the manual we are sorting
 * @param manual_len the length of the manual
 */
void order_incorrect_manual(uint8_t *manual, uint8_t manual_len)
{
    // holds swapping var
    uint8_t swap;
    for (uint8_t i = 0; i < manual_len; i++)
    {
        // sorts array
        for (uint8_t j = 0; j < manual_len - i - 1; j++)
        {
            // swap if values are out of order
            if (page_rules[manual[j]][manual[j + 1]])
            {
                // save the greater value
                swap = manual[j];
                // swap the two
                manual[j] = manual[j + 1];
                manual[j + 1] = swap;
            }
        }
    }
}

/**
 * given a manual, it will check to see if it is valid given
 *      the predefined rules we found
 *
 * @param manual the manual we are validating
 * @param manual_len the length of the manual
 * @returns 0 if invalid, else 1
 */
uint8_t is_manual_valid(uint8_t *manual, uint8_t manual_len)
{
    // we can reduce the number of pages we cehck each iteration
    //      as we can assume by the end, we've checked for all
    //      former pages
    // ex:  * * * * *
    //      x * * * *
    //      x x * * *
    //      x x X * *
    //      These are all the checks we need to do. An x represents
    //          an index we already checked
    for (uint8_t i = 0; i < manual_len - 1; i++)
    {
        // get the rules for the current page (any pages in this array HAVE TO BE after this page)
        uint8_t *cur_page = page_rules[manual[i]];
        for (uint8_t j = i + 1; j < manual_len; j++)
        {
            // if appears in the current page's rules, it means the current
            //      page was supposed to come before it, and inheritly, if
            //      this page is being read AFTER cur_page, we've failed
            if (cur_page[manual[j]])
                return 0;
        }
    }

    return 1;
}

/**
 * reads through a given line/manual, and after parsing, determines
 *      if it is valid
 *
 * @param fp the file we are reading from
 * @returns 0 if invalid, 100 if we reached EOF, or else the value of the middle page
 */
uint8_t read_manual(FILE *fp)
{
    uint8_t ret = 0;
    // get manual len
    uint8_t manual_len = find_num_pages_in_manual(fp);

    // if 0, it means EOF. return EOF value
    if (!manual_len)
        return 100;

    uint8_t manual_pos = 0;
    // for parsing
    char buf[3];
    uint8_t buf_pos = 0;
    char c = 0;

    // allocate memory for manual
    uint8_t *manual = (uint8_t *)malloc(sizeof(uint8_t) * manual_len);
    // parse the array
    do
    {
        fread(&c, sizeof(char), 1, fp);

        if (c == ',' || c == '\n')
        {
            // save the parsed value
            buf[buf_pos] = 0;
            manual[manual_pos++] = atoi(buf);
            buf_pos = 0;
        }
        else
        {
            // save char if not a sperator
            buf[buf_pos++] = c;
        }
    } while (c != '\n');

    // check validity
    ret = is_manual_valid(manual, manual_len);
    // we only want the INVALID middle pages for **part 2**
    //      for **part 1** just swap lines 160 and 165!
    if (ret)
    {
        ret = 0;
    }
    else
    {
        order_incorrect_manual(manual, manual_len);
        ret = manual[manual_len / 2];
    }

    // free the manual
    free(manual);
    return ret;
}

/**
 * parses the rules of a current file, and saves them to the global
 *      rules array
 * this function will stop reading once the breakpoint between rules
 *      and manuals is found
 *
 * @param fp the file to read rules from
 */
void parse_rules(FILE *fp)
{
    // for parsing
    char buf[3];
    char c;
    uint8_t buf_pos = 0;
    // for determining which manual we are adding a rule to
    // X|Y => cur_page|page_gt_cur_page
    uint8_t cur_page;
    uint8_t page_gt_cur_page;
    // this helps determine when we've stopped reading rules
    uint8_t newlines_in_a_row = 0;

    while (newlines_in_a_row != 2)
    {
        fread(&c, sizeof(char), 1, fp);
        // parse the page that is less than the upcoming page
        if (c == '|')
        {
            buf[buf_pos] = 0;
            cur_page = atoi(buf);
            buf_pos = 0;
        }
        // save the rule IF we didn't just read a newline
        else if (!newlines_in_a_row && c == '\n')
        {
            // increment the num of newlines we've read
            newlines_in_a_row++;
            // parse num
            buf[buf_pos] = 0;
            page_gt_cur_page = atoi(buf);
            buf_pos = 0;
            // set page_gt_cur_page[cur_page] = 1;
            page_rules[page_gt_cur_page][cur_page] = 1;
            // this means that there is a rule set for page_gt_cur_page
            //      that cur_page HAS to come before it
        }
        // if this is an empty line (two newlines in a row) break
        else if (newlines_in_a_row && c == '\n')
        {
            break;
        }
        // add char to buf
        else
        {
            buf[buf_pos++] = c;
            newlines_in_a_row = 0;
        }
    }
}

/**
 * runs advent of code day 5
 */
void main()
{
    // open file
    FILE *fp;
    fp = fopen("./input", "rb");

    // parse all rules!
    parse_rules(fp);
    // erm, go thru and parse every manual please 🤓☝️
    uint16_t tot = 0;
    uint8_t cur_value = 0;
    // add up all the middle pages
    while ((cur_value = read_manual(fp)) != 100)
        tot += cur_value;

    printf("Middle Pages of Valid Manuals Added Up: %d", tot);

    // close file!
    fclose(fp);
}

// 4556 too low
// 4785 too high