#include "messages.h"

#include <stdio.h>
#include <stdarg.h>

int show_message(const char *fs, ...)
{
    va_list args;
    va_start(args, fs);
    int out = vfprintf(MESSAGES_STREAM, fs, args);
    va_end(args);
    return out;
}
