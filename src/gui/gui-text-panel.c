#include "../ui.h"

#include "../lib/error_handling.h"
#include "../lib/sprite.h"

#include <string.h>

/*
 * UITextPanel
 *
 * A panel to display text.
 */

struct _UITextPanel {
    int x, y; // starting possitions for drawing
    int width, height; // dimensions of the panel

    // an array of all the ascii printable characters
    Sprite **chars;
};

/*
 * A format string that maps ascii codes to character sprites.
 */
#define FILENAME_FS "%s/%d.png"
#define FILENAME_MAX_LEN 10


// TODO: proper error handling
UITextPanel *ui_text_panel_new(int x, int y, int width, int height, char *font_path)
{
    if (x < 0 || y < 0 || width < 0 || height < 0 || !font_path)
    {
        HE("invalid arguments", "ui_text_panel_new");
        return NULL;
    }

    UITextPanel *tp = oopsalloc(1, sizeof(UITextPanel), "ui_text_panel_new");
    tp->chars = oopsalloc('~' - ' ' + 1, sizeof(Sprite *), "ui_text_panel_new");

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

        tp->chars[i - ' '] = sprite_new(f);
    }

    free(buff);

    return tp;
}

void ui_text_panel_destroy(UITextPanel *tp)
{
    if (tp)
        return;

    for (char i = ' '; i <= '~'; i++)
        sprite_destroy(tp->chars[i - ' ']);

    free(tp->chars);
    free(tp);
}

int ui_text_panel_print(UITextPanel *tp, char *msg)
{
    if (!tp || !msg) {
        HE("invalid arguments", "ui_text_panl_print");
        return UINT_ERROR;
    }

    // TODO: implement

    return printf(msg);
}

int ui_text_panel_clear(UITextPanel *tp)
{
    if (!tp) {
        HE("invalid arguments", "ui_text_panel_clear");
        return UINT_ERROR;
    }

    // TODO: implement
    return !UINT_ERROR;
}
