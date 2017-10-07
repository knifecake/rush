/*
 * Define ERROR handling constants.
 */

/*
 * PINT_ERROR should be returned by functions that encounter an error and are
 * suposed to return a positive integer, such as the number of operations
 * performed. Please take care not to return PINT_ERROR if your function may
 * return negative integers or if your return type is unsigned.
 */
#define PINT_ERROR -1

/*
 * Instructs handle_error to output error information to the given file or
 * stream.
 */
#define ERROR_LOG_FILE stderr

/*
 * This functions should handle all errors encountered by the program. This
 * function can either be run with one or four arguments. The first one is
 * always required. If the first one ends with a new line ('\n'), then the rest
 * are ignored and the given error message is handled. Else, the rest are used
 * to craft an appropriate error message. The arguments handle_error may
 * receive are the following:
 *
 *  - const char *error_msg
 *  - cont char *function_name
 *  - const char *file_name
 *  - int line_number
 *
 * file_name and line_number can be automatically obtained thanks to the
 * preprocessor. Just call the function with __FILE__ and __LINE__ as these
 * arguments, e.g. handle_error("some error message", "load_resources",
 * __FILE__, __LINE__); Do not explicitly state neither of the previous, as
 * they may change over time.
 *
 * Please note that because this function does not belong in the standard
 * library, the compiler will not be as descriptive as it is with fprintf on
 * its error messages.
 */
void handle_error(const char *error_msg, ...);
