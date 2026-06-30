#include "splashkit.h"

/**
 * Print text to the terlowal a number of times, allowing repeated patterns.
 * 
 * @param text the text to repeat printing
 * @param times the number of times to repeat the text
 * @param with_newline when true, this adds a newline to the end of the output
 */

void print_repeat(string text, int times, bool with_newline)
{
    int i = 0;

    while(i < times)
    {
        write(text);
        i++;
    }
    
    if (with_newline)
    {
        write_line("\n");
    }
}

/**
 * Print a line onto the Terlowal.
 * 
 * @param length the length of the line to print
 */

void print_line(int length)
{
    int i = 0;

    while (i < length)
    {
        write("-");
        i++;
    }

    write_line("\n");
}


/**
 * Display a promp, read the string entered at the terminal, and
 * returns this string to the caller. This can be used to read values
 * from user.
 * 
 * @param prompt The messge to show to the user
 * @returns The string entered at the terminal
 */

string read_string(string prompt)
{
    write(prompt);
    return read_line();
}

/**
 * Read an integer frmo the user
 * 
 * @param the messge to show the user
 * @returns the integer entered
 */

int read_integer(string prompt)
{
    string user_input = read_string(prompt);

    while(!is_integer(user_input))
    {
        write_line("Please enter a whole number.");
        user_input = read_string(prompt);
    }

    return to_integer(user_input);
}

/**
 * Read an integer from the user within a given range.
 * 
 * @param prompt the message to show the user
 * @param low an integer representing the smallest value to return
 * @param high an integer representing the largest value to return
 * @returns the integer value
 */

int read_integer(string prompt, int low, int high)
{
    int value = read_integer(prompt);

    while (value < low || value > high)
    {
        write_line("Please enter a value between " + to_string(low) + " and " + to_string(high));
        value = read_integer(prompt);
    }

    return value;
}

/**
 * Swaps the values of the two integer varaibles passed in.
 * 
 * @param x the first integer variable to swap
 * @param y the second integer variable to swap
 */
void swap(int &x, int &y)
{
    int temp = y;
    y = x;
    x = temp;
}
