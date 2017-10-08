#include "sprite.h"

#include <stdlib.h>
#include <math.h>

#include "error_handling.h"

struct _Sprite8 {
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
    RGBQUAD *pallete;
    BYTE *image;
};

/*
 * Returns the pixel at (x,y). Please note that Sprites are indexed starting at
 * 1, ending at s->bi.biWidth and s->bi.biHeight, respectively.
 */
BYTE _sprite_pixel_at(Sprite *s, int x, int y);

Sprite *sprite_new(FILE *img)
{
    if (!img) {
        handle_error("sprite_new: invalid params (no file given)\n");
        return NULL;
    }

    Sprite *s = malloc(sizeof(Sprite));
    if (!s) {
        handle_error("sprite_new: could not initialize sprite, out of memory\n");
        return NULL;
    }

    fread(&s->bf, sizeof(BITMAPFILEHEADER), 1, img);

    fread(&s->bi, sizeof(BITMAPINFOHEADER), 1, img);

    if (s->bf.bfType != 0x4d42) {
        handle_error("sprite_new: file type not supported, this is not a BMP3!\n");
        free(s); return NULL;
    }
    if (s->bi.biBitCount != 8) {
        handle_error("sprite_new: image type not supported, the file is not enconded in 8-bit color\n");
        free(s); return NULL;
    }
    if (s->bi.biCompression != 0) {
        handle_error("sprite_new: image type not supported, the file is not uncompressed\n");
        free(s); return NULL;
    }

    /*  TODO: enable support for less than 8 bit color */
    if (s->bi.biBitCount <= 8) {

        int colors = pow(2, s->bi.biBitCount);
        s->pallete = malloc(colors * sizeof(RGBQUAD));
        if (!s->pallete) {
            handle_error("sprite_new: could not read color palete from file, out of memory\n");
            free(s); return NULL;
        }

        fread(s->pallete, sizeof(RGBQUAD), colors, img);
    }

    // move to the beginning of the pixel array
    fseek(img, s->bf.bfOffBits, SEEK_SET);

    // allocate space for our image
    s->image = malloc(s->bi.biSizeImage * sizeof(s->image));
    if (!s->image) {
        handle_error("sprite_new: could not read image, out of memory\n");
        free(s->pallete); free(s); return NULL;
    }

    fread(s->image, sizeof(s->image), s->bi.biSizeImage, img);

    return s;
}

void sprite_destroy(Sprite *s)
{
    if (!s) return;

    free(s->pallete);
    free(s->image);
    free(s);
}

void sprite_draw(Sprite *s, int x, int y)
{
    if (!s || x < 1 || y < 1) {
        handle_error("sprite_draw: invalid parameters\n");
        return;
    }

    for (int j = 1; j <= s->bi.biHeight; j++) {
        for (int i = 1; i <= s->bi.biWidth; i++) {
            BYTE p = _sprite_pixel_at(s, i, j);
            printf("\033[38;2;%d;%d;%dm\u2588\u2588",
                    s->pallete[p].rgbRed,
                    s->pallete[p].rgbGreen,
                    s->pallete[p].rgbBlue);
            if (i % s->bi.biWidth == 0) printf("\n");
        }
    }
    printf("\033[0m");
}

BYTE _sprite_pixel_at(Sprite *s, int x, int y) {
    if (!s || x < 1 || y < 1 || x > s->bi.biWidth || y > s->bi.biHeight) {
        handle_error("sprite_draw: invalid parameters\n");
        return 0;
    }

    return s->image[(x - 1) * s->bi.biWidth + (y - 1)];
}