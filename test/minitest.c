#include "minitest.h"

/*
 * Have any tests failed? How many tests were run?
 */
static int _minitest_failed_tests, _minitest_run_tests;

typedef enum {green, red} color;

void cprintf(color c, char *format);

void assert(char *desc, bool boolean)
{
    if (boolean)
        cprintf(green, desc);
    else {
        _minitest_failed_tests++;
        cprintf(red, desc);
    }

    printf("\n");
    _minitest_run_tests++;
}

int failed_tests(void)
{
    printf("finished: %d out of %d tests failed\n", _minitest_failed_tests, _minitest_run_tests);
    return _minitest_failed_tests;
}

void cprintf(color c, char *format)
{
    if (c == red)
        printf("\e[0;31m:( %s\e[0m", format);
    else
        printf("\e[0;32m:) %s\e[0m", format);
}
