#ifndef __UI_FONT__
#define __UI_FONT__

#include "lib/sprite.h"

typedef struct _UIFont UIFont;

/*
 * Loads all printable ASCII characters from the font path as sprites.
 */
UIFont *ui_font_new(const char *font_path);

/*
 * Returns the sprite associated with the character a.
 */
Sprite *ui_font_get_char_sprite(UIFont *font, char a);

int ui_font_get_char_width(UIFont *font);
int ui_font_get_char_height(UIFont *font);

/*
 * You need to specify where will it start printing for coordinates x>0; y>0.
 * You also need to specify how many chars you want to write, for example:
 *
 * sprite_write(stdout,font,"Hello World", 0, 0, 7);
 * will only print the seven first characteres it will print then:
 * Hello W
 * @ coordinates (1,1) [This is the top-left corner]
 *
 * In case you want to print the whole string please use strlen.
 * In the previous example it should be:
 *
 * sprite_write(stdout, font, "Hello World", 0, 0, strlen("Hello World"));
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
 * sprite_write(stdout,font,"Hello\nWorld", 0, 0, 11);
 *
 * Please notice that strlen("Hello\nWorld") will do the work again.
 */
void ui_font_print(FILE *fp, UIFont *font, char *string, int x0, int y0, int maxsize);

void ui_font_destroy(UIFont *font);

#endif
