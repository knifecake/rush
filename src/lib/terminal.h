#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include <stdbool.h>

/*
 * Theese constants are returned when gc_read_key encounters an arrow-key.
 */
#define DOWN_ARROW  -1
#define UP_ARROW    -2
#define LEFT_ARROW  -3
#define RIGHT_ARROW -4

/*
 * Returned by gc_read_key in case it finds something that looks like an arrow
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
int gc_read_key(FILE *s);

/*
 * Returns true if the given key is one of the macros defined above.
 */
bool gc_is_arrow_key(int k);

/*
 * Saves current terminal state and adjusts terminal to enable gameplay.
 */
void gc_terminal_setup(FILE *s);

/*
 * Restores initial terminal state.
 */
void gc_terminal_teardown(FILE *s);

#endif
