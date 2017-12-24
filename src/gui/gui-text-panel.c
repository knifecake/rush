#include "../ui.h"

#include "../lib/error_handling.h"
#include "../lib/sprite.h"

#include <string.h>
#include <unistd.h>

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

    // the width and height of the tallest letter: A
    int char_width;
    int char_height;

    // a small delay between characters gives the impresion of a typewriter effect
    int typewriter_effect; // use us

    char *msg;
};

/*
 * A format string that maps ascii codes to character sprites.
 */
#define FILENAME_FS "%s/%d.png"
#define FILENAME_MAX_LEN 10


void _ui_text_panel_draw(UITextPanel *tp)
{
    if (!tp)
        return;

    int x = tp->x, y = tp->y;
    fprintf(stdout, "\033[255;255;255m");
    fprintf(stdout, "\033[%d;%dH", y, x);

    // print the top border
    for (; x < tp->width + tp->x; x++)
        fprintf(stdout, "+");

    // print the side borders
    for (y = tp->y + 1; y < tp->height + tp->y - 1; y++) {
        x = tp->x;
        fprintf(stdout, "\033[%d;%dH", y, x);
        fprintf(stdout, "+");
        for (; x < tp->width + tp->x - 1; x++)
            fprintf(stdout, " ");
        fprintf(stdout, "+");
    }

    x = tp->x;
    y = tp->y + tp->height;
    fprintf(stdout, "\033[%d;%dH", y, x);

    for (; x < tp->width + tp->x; x++)
        fprintf(stdout, "+");
}

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

    tp->x = x;
    tp->y = y;
    tp->width = width;
    tp->height = height;

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

    // calculate width and height of the tallest letter
    tp->char_width = sprite_get_w(tp->chars['A' - ' ']);
    tp->char_height = sprite_get_h(tp->chars['A' - ' ']);

    tp->typewriter_effect = 30000;

    _ui_text_panel_draw(tp);

    ui_text_panel_print(tp, "Hello world");

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

    int x = tp->x + 3, y = tp->y + 3;
    for (int i = 0; i < strlen(msg); i++)
    {
        // if character is not printable continue
        if (msg[i] < ' ' || msg[i] > '~')
            continue;

        // if it is a space, leave room and move left
        if (msg[i] == ' ') {
            x += tp->char_width;
            continue;
        }

        // move down a bit to align the baseline of the characters
        sprite_draw(stdout, tp->chars[msg[i] - ' '], x, y + tp->char_height - sprite_get_h(tp->chars[msg[i] - ' ']));

        x += sprite_get_w(tp->chars[msg[i] - ' ']) + 1;

        usleep(tp->typewriter_effect);
    }

    return strlen(msg);
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
