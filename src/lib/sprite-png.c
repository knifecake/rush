#include "sprite.h"

#include <stdlib.h>
#include <math.h>
#include <png.h>

#include "error_handling.h"

struct _SpritePNG {
    png_structp png;
    png_infop info;

    int height, width;
    png_byte color_type, bit_depth;
    png_byte **rows;
};

#define RGB_RED     0
#define RGB_GREEN   1
#define RGB_BLUE    2
#define RGB_ALPHA   3
#define BYTES_PER_PIXEL 4

png_byte *_sprite_pixel_at(Sprite *s, int x, int y);

Sprite *sprite_new(FILE *img)
{
    if (!img) {
        HE("invalid params (no file given)", "sprite_new")
        return NULL;
    }

    Sprite *s = oopsalloc(1, sizeof(Sprite), "sprite_new");
    s->png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!s->png) {
        HE("cannot create sprite, unable to read png", "sprite_new")
        return NULL;
    }

    s->info = png_create_info_struct(s->png);
    if (!s->info) {
        HE("cannot create sprite, unable to read png", "sprite_new")
        png_destroy_read_struct(&s->png, &s->info, NULL);
        return NULL;
    }

    // TODO: understand jumping arround on error
    if (setjmp(png_jmpbuf(s->png))) {
        HE("error during read_image", "sprite_new")
        png_destroy_read_struct(&s->png, &s->info, NULL);
        abort();
    }

    png_init_io(s->png, img);

    png_read_info(s->png, s->info);
    s->width = png_get_image_width(s->png, s->info);
    s->height = png_get_image_height(s->png, s->info);
    s->color_type = png_get_color_type(s->png, s->info);
    s->bit_depth = png_get_bit_depth(s->png, s->info);


    if(s->bit_depth == 16)
        png_set_strip_16(s->png);

    if(s->color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(s->png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(s->color_type == PNG_COLOR_TYPE_GRAY && s->bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(s->png);

    if(png_get_valid(s->png, s->info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(s->png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if(s->color_type == PNG_COLOR_TYPE_RGB ||
            s->color_type == PNG_COLOR_TYPE_GRAY ||
            s->color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(s->png, 0xFF, PNG_FILLER_AFTER);

    if(s->color_type == PNG_COLOR_TYPE_GRAY ||
            s->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(s->png);

    png_read_update_info(s->png, s->info);

    s->rows = (png_bytep*) oopsalloc(sizeof(png_bytep), s->height, "sprite_new");

    for (int y = 0; y < s->height; y++) {
        s->rows[y] = (png_byte *)oopsalloc(png_get_rowbytes(s->png, s->info), sizeof(char), "sprite_new");
        if (!s->rows[y]) {
            HE("cannot read png, out of memory", "sprite_new")
            for (int i = 0; i < y; free(s->rows[i++]));
            free(s->rows); return NULL;
        }
    }

    png_read_image(s->png, s->rows);

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
    png_destroy_read_struct(&s->png, &s->info, NULL);
    free(s);
}

/*
 * Draws a sprite starting at x0, y0.
 *
 * TODO: we're still not drawing at (x0,y0), but rather at the cursor possition.
 */
void sprite_draw(FILE *f, Sprite *s, int x0, int y0)
{
    if (!s || x0 < 0 || y0 < 0) {
        HE("invalid params", "sprite_draw")
        return;
    }
    x0++;
    y0++;
    x0 *= 2;
    x0--;

    // move to (x0, y0)
    fprintf(f, "\033[%d;%dH", y0, x0);

    for (int y = 0; y < s->height; y++) {
        for (int x = 0; x < s->width; x++) {
            png_byte *p = _sprite_pixel_at(s, x, y);
            if (p[RGB_ALPHA] == 255)
                fprintf(f, "\033[38;2;%d;%d;%dm\u2588\u2588", p[RGB_RED], p[RGB_GREEN], p[RGB_BLUE]);
            else
                fprintf(f, "\033[%d;%dH", y0 + y, x0 + 2 * (x + 1));

            /* fprintf(f, "[%d][%d] = RGBA(%d, %d, %d, %d)\n", x, y, p[RGB_RED], p[RGB_GREEN], p[RGB_BLUE], p[RGB_ALPHA]); */
        }
        fprintf(f, "\n");
        fprintf(f, "\033[%d;%dH", y0 + y + 1, x0);
    }

    printf("\033[0m");
}
png_byte *_sprite_pixel_at(Sprite *s, int x, int y)
{
    if (!s || x < 0 || y < 0 || x > s->width || y > s->height) {
        HE("invalid params", "_sprite_pixel_at")
        return NULL;
    }

    return &s->rows[y][x * BYTES_PER_PIXEL];
}

int sprite_get_h(Sprite *s)
{
    if (!s) return UINT_ERROR;

    return s->height;
}

int sprite_get_w(Sprite *s)
{
    if (!s) return UINT_ERROR;

    return s->width;
}
