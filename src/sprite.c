#include "sprite.h"

#include <stdlib.h>
#include <math.h>

#include "error_handling.h"

struct _Sprite24 {
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
    RGBTRIPLE **image;
};

/*
 * Returns the pixel at (x,y). Please note that Sprites are indexed starting at
 * 1, ending at s->bi.biWidth and s->bi.biHeight, respectively.
 */
RGBTRIPLE *_sprite_pixel_at(Sprite *s, int x, int y);

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
    if (s->bi.biBitCount != 24) {
        handle_error("sprite_new: image type not supported, the file is not enconded in 24-bit color\n");
        free(s); return NULL;
    }
    if (s->bi.biCompression != 0) {
        handle_error("sprite_new: image type not supported, the file is not uncompressed\n");
        free(s); return NULL;
    }

    // move to the beginning of the pixel array
    fseek(img, s->bf.bfOffBits, SEEK_SET);

    // determine padding for scanlines
    int padding = (4 - (s->bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // allocate space for our image
    s->image = malloc(s->bi.biHeight * sizeof(RGBTRIPLE *));
    if (!s->image) {
        handle_error("sprite_new: could not read image, out of memory\n");
        free(s); return NULL;
    }

    for (int i = 0; i < s->bi.biHeight; i++) {
        s->image[i] = malloc(s->bi.biWidth * sizeof(RGBTRIPLE));
        if (!s->image[i]) {
            handle_error("sprite_new: could not read image, out of memory\n");
            for (int k = 0; k < i; free(s->image[k++]));
            free(s->image); free(s);
        }

        for (int j = 0; j < s->bi.biWidth; j++)
            fread(&s->image[i][j], sizeof(RGBTRIPLE), 1, img);

        // skip over padding
        fseek(img, padding, SEEK_CUR);
    }

    return s;
}

void sprite_destroy(Sprite *s)
{
    if (!s) return;

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
            RGBTRIPLE *p = _sprite_pixel_at(s, i, j);
            printf("\033[38;2;%d;%d;%dm\u2588\u2588",
                    p->rgbtRed,
                    p->rgbtGreen,
                    p->rgbtBlue);
            if (i % s->bi.biWidth == 0) printf("\n");
        }
    }
    printf("\033[0m");
}

RGBTRIPLE *_sprite_pixel_at(Sprite *s, int x, int y) {
    if (!s || x < 1 || y < 1 || x > s->bi.biWidth || y > s->bi.biHeight) {
        handle_error("sprite_draw: invalid parameters\n");
        return NULL;
    }

    // TODO: understand why this works
    return &s->image[s->bi.biHeight - y][x - 1];
}
