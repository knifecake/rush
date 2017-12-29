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
    UIRect outer_dim;

    // TODO: these are disabled for easier placement
    UIRect inner_dim;

    UIFont *font;
    int char_width, char_height;

    // a small delay between characters gives the impresion of a typewriter effect
    int typewriter_effect; // use us

    // the ammout of space left between the borders and the text-box
    int padding;

    char *msg;
};


// TODO: proper error handling
UITextPanel *ui_text_panel_new(UIRect outer_dim, UIFont *font)
{
    if (!font)
    {
        HE("invalid arguments", "ui_text_panel_new");
        return NULL;
    }

    UITextPanel *tp = oopsalloc(1, sizeof(UITextPanel), "ui_text_panel_new");

    tp->font = font;
    tp->char_width = ui_font_get_char_width(tp->font);
    tp->char_height = ui_font_get_char_height(tp->font);

    // TODO: make this parameter configurable
    tp->padding = 1;

    // copy outer dimensions
    tp->outer_dim = outer_dim;

    // calculate inner dimensions with padding
    /* tp->inner_dim.x = tp->outer_dim.x + 2 * tp->padding; */
    /* tp->inner_dim.y = tp->outer_dim.y + tp->padding; */
    /* tp->inner_dim.height = tp->outer_dim.height - 2 * tp->padding; */
    /* // account for both margins and for the double x pixels */
    /* tp->inner_dim.width = tp->outer_dim.width - 2 * 2 * tp->padding; */
    // TODO: temporarily (or not) disable inner dimmensions
    tp->inner_dim = outer_dim;


    // TODO: make this parameter adjustable
    tp->typewriter_effect = 0;

    // Optionally, draw a border for debugging
    /* ui_draw_rect(tp->outer_dim, '+'); */
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

    int x = tp->inner_dim.x, y = tp->inner_dim.y;
    for (int i = 0; i < strlen(msg); i++)
    {
        // if this does not fit horizontally, move down a line
        // alternatively, it this character is a new line, do the same
        if (x + tp->char_width > tp->inner_dim.x + tp->inner_dim.width || msg[i] == '\n') {
            x = tp->inner_dim.x;
            y += tp->char_height + tp->padding;
        }

        // if this does not fit vertically, stop printing
        if (y + tp->char_height >= tp->inner_dim.y + tp->inner_dim.height) {
            break;
        }

        // if character is not printable continue
        if (msg[i] < ' ' || msg[i] > '~')
            continue;

        // if it is a space, leave room and move right
        if (msg[i] == ' ') {
            x += tp->char_width;
            continue;
        }

        // move down a bit to align the baseline of the characters
        Sprite *ch = ui_font_get_char_sprite(tp->font, msg[i]);
        if (!ch) {
            HE("this font does not support this character", "ui_text_panel_print");
            continue;
        }
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

    ui_clear_rect(tp->inner_dim);
    return !UINT_ERROR;
}
