/*
 * PPROG Game – Morzilla Firefox 2018
 *
 * Authors: Miguel Baquedano, Sergio Cordero, Elias Hernandis
 *          and Rafael Sánchez.
 *
 * Lead author: <replace me>
 */


#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <stdio.h>

/*
 * Sprite: a library for drawing bitmaps on the terminal.
 *
 * Sprite enables you to load a special kind of bitmap file into memory. From
 * there you can manipulate it, associate it with other entities or print it on
 * screen. With a few caveats:
 *  - displaying bitmaps assumes a terminal capable of displaying 24-bit color
 *    through ANSI escape codes
 *  - setting a font size small enough to get any decent resolution means you'll
 *    have to draw the text yourself
 *  - valid sprites are those understood by libpng on your system
 */

/*
 * Define sprites as PNGs. We only support that at the
 * moment.
 */
typedef struct _SpritePNG Sprite;


/*
 * Returns a wrapper around an image file.
 *
 * img must be a FILE * pointer to a PNG image file
 *
 *  Returns NULL on error, a pointer to a Sprite on success.
 */
Sprite *sprite_new(FILE *img);

/*
 * Frees a sprite, does nothing if given a NULL pointer.
 */
void sprite_destroy(Sprite *s);

/*
 * Draws a sprite.
 *
 * Starts at x,y, so the stream given should be a terminal.
 */
void sprite_draw(FILE *f, Sprite *s, int x, int y);

/*
 * Returns the height of the given sprite, UINT_ERROR on error.
 */
int sprite_get_h(Sprite *s);

/*
 * Returns the width of the given sprite, UINT_ERROR on error.
 */
int sprite_get_w(Sprite *s);
#endif
