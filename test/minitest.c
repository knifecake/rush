#include "minitest.h"

/*
 * Have any tests failed?
 */
static int _minitests_failed_tests;

typedef enum {green, red} color;

void cprintf(color c, char *format);

void assert(char *desc, bool boolean)
{
    if (boolean)
        cprintf(green, desc);
    else {
        _minitests_failed_tests++;
        cprintf(red, desc);
    }

    printf("\n");
}

int failed_tests(void)
{
    return _minitests_failed_tests;
}

void cprintf(color c, char *format)
{
    if (c == red)
        printf("\e[0;31m:( %s\e[0m", format);
    else
        printf("\e[0;32m:) %s\e[0m", format);
}
