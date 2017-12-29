#include "../ui.h"

#include <stdio.h>

void ui_clear_rect(UIRect r)
{
    ui_draw_rect(r, ' ');
}

void ui_draw_rect(UIRect r, char border)
{
    int i = r.x, j = r.y;

    // TODO: setting a white color, maybe make this customizable
    fprintf(stdout, "\033[255;255;255m");

    fprintf(stdout, "\033[%d;%dH", j, 2 * i);

    // print the top border
    for (; i < r.x + 2 * r.width; i++)
        fputc(border, stdout);

    // print the side borders
    for (j = r.y + 1; j < r.height + r.y; j++) {
        i = r.x;
        fprintf(stdout, "\033[%d;%dH", j, 2 * i);
        fputc(border, stdout);
        for (; i < r.x + 2 * r.width - 1; i++)
            fputc(' ', stdout);
        fputc(border, stdout);
    }

    i = r.x;
    j = r.y + r.height;
    fprintf(stdout, "\033[%d;%dH", j, 2 * i);

    for (; i < r.x + 2 * r.width; i++)
        fputc(border, stdout);

}
