#ifndef UTILITIES_H
#define UTILITIES_H

#include "splashkit.h"


void print_line(int length);
void print_repeat(string text, int times, bool with_newline);
string read_string(string prompt);
int read_integer(string prompt);
int read_integer(string prompt, int min, int max);
void swap(int &x, int &y);

#endif
