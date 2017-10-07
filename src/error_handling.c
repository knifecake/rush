#include "error_handling.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

void handle_error(const char *error_msg, ...)
{
    if (!error_msg) {
        handle_error("handle_error: called without an error message\n");
        return;
    }

    int num_args = (error_msg[strlen(error_msg) - 1] == '\n') ? 1 : 4;

    va_list args;
    if (num_args > 1) {
        va_start(args, error_msg);
        const char *func_name   = va_arg(args, const char *);
        const char *file_name   = va_arg(args, const char *);
        int line_no             = va_arg(args, int);

        fprintf(ERROR_LOG_FILE, "%s at %s:%d: %s\n", func_name, file_name, line_no, error_msg);
    }
    else
    {
        fprintf(ERROR_LOG_FILE, "%s", error_msg);
    }
}
