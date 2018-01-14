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
    if(border == ' '){
      fprintf(stdout, "\033[48;2;%d;%d;%dm",78, 74, 78);
    }else{
      fprintf(stdout, "\033[%d;%d;%dm",78, 74, 78);
    }
    fprintf(stdout, "\033[%d;%dH", j, 2 * i);

    // print the top border
    for (; i < r.x + 2 * r.w; i++)
        fputc(border, stdout);

    // print the side borders
    for (j = r.y + 1; j < r.h + r.y; j++) {
        i = r.x;
        fprintf(stdout, "\033[%d;%dH", j, 2 * i);
        fputc(border, stdout);
        for (; i < r.x + 2 * r.w - 1; i++)
            fputc(' ', stdout);
        fputc(border, stdout);
    }

    i = r.x;
    j = r.y + r.h;
    fprintf(stdout, "\033[%d;%dH", j, 2 * i);

    for (; i < r.x + 2 * r.w; i++)
        fputc(border, stdout);

}
