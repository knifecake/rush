#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "error_handling.h"

#define MAXBUFF 100

#define RED 0
#define GREEN 1
#define BLUE 2
#define ALPHA 3

typedef struct _Sprite Sprite;

/*
 * Given a filename, creates a new sprite with the image on it.
 * Returns NULL on error.
 */
Sprite *sprite_new(const char *);

/*
 * Frees the memory allocated by sprite_new.
 */
void sprite_destroy(Sprite *);

/*
 * Given a file output, a sprite, and the intial coordinates of the console
 * (at least x0 = 1 and y0 = 1), prints the image on the output.
 */
void sprite_draw(FILE *, Sprite *, int , int );
