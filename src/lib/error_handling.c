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
        va_end(args);

        // show header of message in yellow
        fprintf(ERROR_LOG_FILE, "\e[0;33m%s at %s:%d:\e[0m %s\n", func_name, file_name, line_no, error_msg);
    }
    else
    {
        fprintf(ERROR_LOG_FILE, "%s", error_msg);
    }
}

void log_msg(const char *msg, ...)
{
    // ignore calls to this function if we're debugging
    if (DEBUG == false) return;

    if (DEBUG_LOG_FILE == ERROR_LOG_FILE)
        fprintf(DEBUG_LOG_FILE, "DEBUG: ");

    int num_args = (msg[strlen(msg) - 1] == '\n') ? 1 : 4;

    va_list args;
    if (num_args > 1) {
        va_start(args, msg);
        const char *func_name   = va_arg(args, const char *);
        const char *file_name   = va_arg(args, const char *);
        int line_no             = va_arg(args, int);
        va_end(args);

        handle_error(msg, func_name, file_name, line_no);
    } else
        handle_error(msg);
}
