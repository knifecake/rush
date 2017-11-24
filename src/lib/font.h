#ifndef __FONT_H__
#define __FONT_H__

#include "sprite.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * The Sprite** will contain the Sprite* Array of chars.
 * You need to specify where will it start printing for coordinates x>1; y>0.
 * You also need to specify how many chars you want to write, for example:
 *
 * sprite_write(stdout,font,"Hello World", 1, 1, 7);
 * will only print the seven first characteres it will print then:
 * Hello W
 * @ coordinates (1,1) [This is the top-left corner]
 *
 * In case you want to print the whole string please use strlen.
 * In the previous example it should be:
 *
 * sprite_write(stdout, font, "Hello World", 1, 1, strlen("Hello World"));
 * However, if the number of chars to write is longer than the word itself, it
 * will only print the exact number of chars.
 *
 * Lastly, '\n' usage is accepted, however please notice this count as a new
 * char so in order to print correctly the next string:
 *
 * Hello
 * World
 *
 * The function will need to be something like:
 * sprite_write(stdout,font,"Hello\nWorld", 1, 1, 11);
 *
 * Please notice that strlen("Hello\nWorld") will do the work again.
 */
 void font_write(FILE *, Sprite **, char *, int, int, int);
#endif
