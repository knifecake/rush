#ifndef __MESSAGES_H__
#define __MESSAGES_H__

/*
 * A library for handling messages to the user in our game.
 *
 * Anytime you need to explain that would appear in the messages section of the
 * UI call the methods in this library.
 */

/*
 * For the moment, we're not using the UI, so we'll raw text. This constant
 * defines where should we print the text.
 */
#define MESSAGES_STREAM stdout

/*
 * Handles a message. Pretty much works on the same way as print, but allows us
 * to change where things are output to.
 */
int show_msg(const char *msg, ...);
#endif
