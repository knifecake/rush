#include "../ui-font.h"

#include "../lib/error_handling.h"
#include <string.h>

struct _UIFont {
    Sprite **chars;

    // some basic character dimensions
    int width, height;
};

/*
 * A format string that maps ascii codes to character sprites.
 */
#define FILENAME_FS "%s/%d.png"
#define FILENAME_MAX_LEN 10

UIFont *ui_font_new(const char *font_path)
{
    if (!font_path) {
        HE("invalid arguments", "ui_font_new");
        return NULL;
    }

    UIFont *font = oopsalloc(1, sizeof(UIFont), "ui_font_new");
    font->chars = oopsalloc('~' - ' ', sizeof(Sprite *), "ui_font_new");

    // load each printable character into a sprite
    // we allocate a buffer approximately the size of the final filename
    char *buff = oopsalloc(FILENAME_MAX_LEN + strlen(font_path), sizeof(char), "ui_text_panel_new");
    for (char i = ' '; i <= '~'; i++)
    {
        // compute filename
        sprintf(buff, FILENAME_FS, font_path, i);

        FILE *f = fopen(buff, "r");

        // skip over characters not in the font
        if (!f)
            continue;

        font->chars[i - ' '] = sprite_new(f);
        fclose(f);
    }

    free(buff);

    // calculate width and height of the tallest letter
    font->width = sprite_get_w(font->chars['A' - ' ']);
    font->height = sprite_get_h(font->chars['A' - ' ']);

    return font;
}

Sprite *ui_font_get_char_sprite(UIFont *font, char a)
{
    if (!font) {
        HE("invalid arguments", "ui_font_get_char_sprite");
        return NULL;
    }

    return font->chars[a - ' '];
}

int ui_font_get_char_width(UIFont *font)
{
    if (!font)
        return UINT_ERROR;
    return font->width;

}

int ui_font_get_char_height(UIFont *font)
{
    if (!font)
        return UINT_ERROR;
    return font->height;
}


void ui_font_destroy(UIFont *font)
{
    if (!font)
        return;

    for (char i = ' '; i <= '~'; i++)
        sprite_destroy(font->chars[i - ' ']);

    free(font->chars);
    free(font);
}
