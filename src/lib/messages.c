#include "messages.h"

#include <stdio.h>
#include <stdarg.h>

#include "../ui.h"

int show_msg(const char *fs, ...)
{
    char buff[1000];
    va_list args;
    va_start(args, fs);
    vsprintf(buff, fs, args);
    va_end(args);
    return ui_show_msg(buff);
}
