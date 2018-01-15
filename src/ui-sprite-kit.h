#include "lib/sprite.h"

typedef struct {
    int x, y;
} SKVector;

typedef struct _SKMinion SKMinion;

/*
 * SKMinion: a particle.
 */
SKMinion *sk_minion_new(UIRect dim, char border, char fill, SKVector speed, Sprite *sprite);

void sk_minion_destroy(SKMinion *m);

void sk_minion_update_position(SKMinion *m, int dx, int dy);

void sk_minion_draw(SKMinion *m);

void sk_minion_move(SKMinion *m, int x, int y);

typedef enum {
    OVERLAPPING,
    TOUCHING_N, // m2 north of m1
    TOUCHING_E, // m2 east of m1
    TOUCHING_S, // m2 south of m1
    TOUCHING_W, // m2 west of m1
    APART,
    ARGUMENT_ERROR
} SKMinionRelativePosition;

SKMinionRelativePosition sk_minion_relative_pos(SKMinion *m1, SKMinion *m2);

void sk_minion_step(SKMinion *m);

UIRect *sk_minion_get_dim(SKMinion *m);
/*
 * SKGru: a group of particles contained in a rectangle.
 */
typedef struct _SKGru SKGru;

SKGru *sk_gru_new(UIRect dim, char border);

void sk_gru_destroy(SKGru *g);

void sk_gru_add_minion(SKGru *g, SKMinion *m);

void sk_gru_draw(SKGru *g);

void sk_gru_next_frame(SKGru *g);

/*
 * True if the minion p is touching or overlaping any other minion in the SKGru
 * g given.
 */
bool sk_gru_minion_collides_with_others(SKGru *g, SKMinion *p);

UIRect *sk_gru_get_dim(SKGru *g);
