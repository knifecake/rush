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

    UIFont *font;
    int char_width, char_height;

    // a small delay between characters gives the impresion of a typewriter effect
    int typewriter_effect; // use us

    // the ammout of space left between the borders and the text-box
    int padding;
    int tb_x, tb_y;
    int tb_height, tb_width;

    char *msg;
};


// TODO: proper error handling
UITextPanel *ui_text_panel_new(int x, int y, int width, int height, UIFont *font)
{
    if (x < 0 || y < 0 || width < 0 || height < 0 || !font)
    {
        HE("invalid arguments", "ui_text_panel_new");
        return NULL;
    }

    UITextPanel *tp = oopsalloc(1, sizeof(UITextPanel), "ui_text_panel_new");

    tp->x = x;
    tp->y = y;
    tp->width = width;
    tp->height = height;
    tp->font = font;
    tp->char_width = ui_font_get_char_width(tp->font);
    tp->char_height = ui_font_get_char_height(tp->font);

    tp->padding = 1;
    tp->tb_x = tp->x + 2 * tp->padding;
    tp->tb_y = tp->y + tp->padding;
    tp->tb_height = tp->height - 2 * tp->padding;
    tp->tb_width = tp->width - 4 * tp->padding;

    tp->typewriter_effect = 0;

    ui_draw_rect(tp->x, tp->y, tp->width, tp->height);
    return tp;
}

void ui_text_panel_destroy(UITextPanel *tp)
{
    if (tp)
        return;

    free(tp);
}

int ui_text_panel_print(UITextPanel *tp, char *msg)
{
    if (!tp || !msg) {
        HE("invalid arguments", "ui_text_panl_print");
        return UINT_ERROR;
    }

    ui_text_panel_clear(tp);

    int x = tp->tb_x, y = tp->tb_y;
    for (int i = 0; i < strlen(msg); i++)
    {
        // if this does not fit horizontally, move down a line
        // alternatively, it this character is a new line, do the same
        if (x + tp->char_width > tp->tb_x + tp->tb_width || msg[i] == '\n') {
            x = tp->tb_x;
            y += tp->char_height + tp->padding;
        }

        // if this does not fit vertically, stop printing
        if (y + tp->char_height >= tp->tb_y + tp->tb_height) {
            break;
        }

        // if character is not printable continue
        if (msg[i] < ' ' || msg[i] > '~')
            continue;

        // if it is a space, leave room and move left
        if (msg[i] == ' ') {
            x += tp->char_width;
            continue;
        }

        // move down a bit to align the baseline of the characters
        Sprite *ch = ui_font_get_char_sprite(tp->font, msg[i]);
        sprite_draw(stdout, ch, x, y + tp->char_height - sprite_get_h(ch));

        x += sprite_get_w(ch) + 1;

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

    ui_clear_rect(tp->tb_x, tp->tb_y, tp->tb_width, tp->tb_height);
    return !UINT_ERROR;
}
