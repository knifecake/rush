#include "../lib/bmp.h"

#include <stdio.h>

/*
 * Sprite: a library for drawing bitmaps on the terminal.
 *
 * Sprite enables you to load a special kind of bitmap file into memory. From
 * there you can manipulate it, associate it with other entities or print it on
 * screen. With a few caveats:
 *  - currently, we only support BMP3 bitmaps which must be uncompressed
 *  - also, bitmaps must be encoded in 8 bit color, and indexed
 *  - displaying bitmaps assumes a terminal capable of displaying 24-bit color
 *    through ANSI escape codes
 */

/*
 * Define sprites as 8-bit indexed BMPs. We only support that at the
 * moment.
 */
typedef struct _Sprite8 Sprite;


/*
 * Returns a wrapper around an image file.
 *
 * img must be a FILE * pointer to an uncompressed, indexed, 8-bit depth BMP3
 * image file.
 *
 * You may generate these kinds of files yourself or you may convert almost any
 * image file to this format by using ImageMagick. Run `convert -compress None
 * -colors 256 original.whatever BMP3:image.bmp` to obtain an appropriate image
 *  file.
 *
 *  Returns NULL on error, a pointer to a Sprite on success.
 */
Sprite *sprite_new(FILE *img);

/*
 * Frees a sprite, does nothing if given a NULL pointer.
 */
void sprite_destroy(Sprite *s);

/*
 * Draws a sprite starting at x, y.
 *
 * TODO: we're still not drawing at (x,y), but rather at the cursor possition.
 */
void sprite_draw(Sprite *s, int x, int y);
