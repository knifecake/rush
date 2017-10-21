#include "minitest.h"

typedef enum {green, red} color;

void cprintf(color c, char *format);

void assert(char *desc, bool boolean)
{
    if (boolean)
        cprintf(green, desc);
    else
        cprintf(red, desc);

    printf("\n");
}

void cprintf(color c, char *format)
{
    if (c == red)
        printf("\e[0;31m:( %s\e[0m", format);
    else
        printf("\e[0;32m:) %s\e[0m", format);
}
