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

void ui_font_destroy(UIFont *font);

#endif
