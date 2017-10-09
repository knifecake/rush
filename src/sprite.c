#include "sprite.h"

#include <stdlib.h>
#include <math.h>

#include "error_handling.h"

struct _SpritePNG {
    png_structp png;
    png_infop info;

    int height, width;
    png_byte color_type, bit_depth;
    png_byte **rows;
    int bytes_per_pixel;
};

png_byte *_sprite_pixel_at(Sprite *s, int x, int y);

Sprite *sprite_new(FILE *img)
{
    if (!img) {
        handle_error("invalid params (no file given)", "sprite_new", __FILE__, __LINE__);
        return NULL;
    }

    Sprite *s = malloc(sizeof(Sprite));
    s->png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!s->png) {
        handle_error("cannot create sprite, unable to read png", "sprite_new", __FILE__, __LINE__);
        return NULL;
    }

    s->info = png_create_info_struct(s->png);
    if (!s->info) {
        handle_error("cannot create sprite, unable to read png", "sprite_new", __FILE__, __LINE__);
        png_destroy_read_struct(&s->png, &s->info, NULL);
        return NULL;
    }

    png_init_io(s->png, img);

    png_read_info(s->png, s->info);
    s->width = png_get_image_width(s->png, s->info);
    s->height = png_get_image_height(s->png, s->info);
    s->color_type = png_get_color_type(s->png, s->info);
    s->bit_depth = png_get_bit_depth(s->png, s->info);

    png_read_update_info(s->png, s->info);


    if (setjmp(png_jmpbuf(s->png)))
            handle_error("error during read_image", "sprite_new", __FILE__, __LINE__);

    s->rows = (png_bytep*) malloc(sizeof(png_bytep) * s->height);
    if (!s->rows) {
        handle_error("cannot read png, out of memory", "sprite_new", __FILE__, __LINE__);
        return NULL;
    }

    for (int y = 0; y < s->height; y++) {
        s->rows[y] = (png_byte*) malloc(png_get_rowbytes(s->png, s->info));
        if (!s->rows[y]) {
            handle_error("cannot read png, out of memory", "sprite_new", __FILE__, __LINE__);
            for (int i = 0; i < y; free(s->rows[i++]));
            free(s->rows); return NULL;
        }
    }

    png_read_image(s->png, s->rows);
    if (png_get_color_type(s->png, s->info) != PNG_COLOR_TYPE_RGB
            && png_get_color_type(s->png, s->info) != PNG_COLOR_TYPE_RGBA) {
        handle_error("cannot read png, color type must be 24-bit RGB or 32-bit RGBA", "sprite_new", __FILE__, __LINE__);
        for (int i = 0; i < s->height; free(s->rows[i++]));
        free(s->rows); free(s); return NULL;
    }

    s->bytes_per_pixel = (png_get_color_type(s->png, s->info) == PNG_COLOR_TYPE_RGBA) ? 4 : 3;

    return s;
}

/*
 * Frees a sprite, does nothing if given a NULL pointer.
 */
void sprite_destroy(Sprite *s)
{
    if (!s) return;

    for (int i = 0; i < s->height; free(s->rows[i++]));
    free(s->rows);
    free(s);
}

/*
 * Draws a sprite starting at x0, y0.
 *
 * TODO: we're still not drawing at (x0,y0), but rather at the cursor possition.
 */
void sprite_draw(Sprite *s, int x0, int y0)
{
    if (!s || x0 < 1 || y0 < 1) {
        handle_error("invalid params", "sprite_draw", __FILE__, __LINE__);
        return;
    }

    for (int y = 1; y <= s->height; y++) {
        for (int x = 1; x <= s->width; x++) {
            png_byte *p = _sprite_pixel_at(s, x, y);
            if (s->bytes_per_pixel == 3 || (s->bytes_per_pixel == 4 && p[3] != 0))
                printf("\033[38;2;%d;%d;%dm\u2588\u2588", p[0], p[1], p[2]);
            else
                printf("\033[0m  ");
            if (x % s->width == 0) printf("\n");
        }
    }

    printf("\033[0m");
}
png_byte *_sprite_pixel_at(Sprite *s, int x, int y)
{
    if (!s || x < 1 || y < 1 || x > s->width || y > s->height) {
        handle_error("invalid params", "_sprite_pixel_at", __FILE__, __LINE__);
        return NULL;
    }

    return &s->rows[y - 1][(x - 1) * s->bytes_per_pixel];
}
