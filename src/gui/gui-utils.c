#include "../ui.h"

#include <stdio.h>

void ui_clear_rect(int x, int y, int width, int height)
{
    for (int j = y; j <= y + height; j++) {
        fprintf(stdout, "\033[%d;%dH", j, 2*x);
        for (int i = x; i <= x + 2 * width; i++)
            fprintf(stdout, " ");
    }
}

void ui_draw_rect(int x0, int y0, int width, int height)
{
    int x = x0, y = y0;
    fprintf(stdout, "\033[255;255;255m");
    fprintf(stdout, "\033[%d;%dH", y, 2 * x);

    // print the top border
    for (; x < x0 + 2 * width; x++)
        fprintf(stdout, "+");

    // print the side borders
    for (y = y0 + 1; y < height + y0; y++) {
        x = x0;
        fprintf(stdout, "\033[%d;%dH", y, 2 * x);
        fprintf(stdout, "+");
        for (; x < x0 + 2 * width - 1; x++)
            fprintf(stdout, " ");
        fprintf(stdout, "+");
    }

    x = x0;
    y = y0 + height;
    fprintf(stdout, "\033[%d;%dH", y, 2 * x);

    for (; x < x0 + 2 * width; x++)
        fprintf(stdout, "+");

}
