#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include <stdbool.h>

/*
 * Theese constants are returned when term_read_key encounters an arrow-key.
 */
#define DOWN_ARROW  -1
#define UP_ARROW    -2
#define LEFT_ARROW  -3
#define RIGHT_ARROW -4

/*
 * Returned by term_read_key in case it finds something that looks like an arrow
 * key but isn't.
 */
#define HERE        -5

/*
 * Reads a key from the file stream given (normally stdin)
 *
 * If the key is an arrow key it will return one of the above constants.
 *
 * Returns 0 on error (if the file stream is NULL).
 */
int term_read_key(FILE *s);

/*
 * Returns true if the given key is one of the macros defined above.
 */
bool term_is_arrow_key(int k);

/*
 * Saves current terminal state and adjusts terminal to enable gameplay.
 */
void term_setup(FILE *s);

/*
 * Restores initial terminal state.
 */
void term_teardown(FILE *s);

#endif