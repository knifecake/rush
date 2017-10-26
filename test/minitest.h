#include <stdio.h>
#include <stdbool.h>

/*
 * Takes a string and a boolean value. If the boolean is true it will print the
 * string in green with a smiley face before it. If the value is false it will
 * print the string in red preceded by a sad face.
 *
 * Use the string to describe what your test does so that when it passes/fails
 * you know what is causing problems.
 */
void assert(char *desc, bool boolean);

/*
 * Returns the number of tests that have failed. This is useful as the return
 * value of the test program, as many continuous integration solutions will
 * take a zero return value as something good, whereas anything else as a sign
 * that something has failed.
 */
int failed_tests(void);
