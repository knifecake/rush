#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "src/lib/terminal.h"
#include "src/lib/error_handling.h"
#include "src/lib/sprite.h"

#define OUTPUT_STREAM stdout

typedef struct {
    // values in rows and columns
    int x, y; // top-left corner position
    int w, h; // width and height
} UIRect;

void _draw_rect(UIRect dim, char border, char fill)
{
    // move to x, y
    fprintf(OUTPUT_STREAM, "\033[%d;%dH", dim.y, dim.x);

    // print the top border
    for (int i = 0; i < 2 * dim.w; i++)
        fputc(border, OUTPUT_STREAM);

    // print the side borders and fill
    for (int j = 1; j < dim.h - 1; j++) {
        fprintf(OUTPUT_STREAM, "\033[%d;%dH", dim.y + j, dim.x);
        fputc(border, OUTPUT_STREAM);
        for (int i = 1; i < 2 * dim.w - 1; i++)
            fputc(fill, OUTPUT_STREAM);
        fputc(border, OUTPUT_STREAM);
    }

    // print the bottom border
    fprintf(OUTPUT_STREAM, "\033[%d;%dH", dim.y + dim.h - 1, dim.x);
    for (int i = 0; i < 2 * dim.w; i++)
        fputc(border, OUTPUT_STREAM);

    fflush(OUTPUT_STREAM);
}

typedef struct {
    int x, y;
} SKVector;

typedef struct {
    UIRect dim;
    SKVector speed;
    char border, fill;
    bool is_drawn, is_speed_updated;
    Sprite *s;
} SKMinion;

SKMinion *sk_minion_new(UIRect dim, char border, char fill, SKVector speed, Sprite *sprite)
{
    SKMinion *m = oopsalloc(1, sizeof(SKMinion), "sk_minion_new");
    m->dim = dim;
    m->speed = speed;
    m->border = border;
    m->fill = fill;
    m->is_drawn = false;
    m->s = sprite;
    return m;
}

void sk_minion_destroy(SKMinion *m)
{
    free(m);
}

void sk_minion_draw(SKMinion *m)
{
    m->is_drawn = true;
    sprite_draw(OUTPUT_STREAM, m->s, m->dim.x / 2, m->dim.y - 1);
    /* _draw_rect(m->dim, m->border, m->fill); */
}

void sk_minion_move(SKMinion *m, int x, int y)
{
    if (!m) {
        HE("invalid arguments", "sk_minion_move");
        return;
    }

    // if minion was drawn, clear it
    if (m->is_drawn)
        _draw_rect(m->dim, ' ', ' ');

    m->dim.x = x;
    m->dim.y = y;
    sk_minion_draw(m);
}

typedef enum {
    OVERLAPPING,
    TOUCHING_N, // m2 north of m1
    TOUCHING_E, // m2 east of m1
    TOUCHING_S, // m2 south of m1
    TOUCHING_W, // m2 west of m1
    APART,
    ARGUMENT_ERROR } SKMinionRelativePosition;

SKMinionRelativePosition sk_minion_relative_pos(SKMinion *m1, SKMinion *m2)
{
    if (!m1 || !m2) {
        HE("invalid arguments", "sk_minion_relative_pos");
        return ARGUMENT_ERROR;
    }

    /*
     * Some possible optimizations for this code:
     *  - Select the smallest of the two minions to iterate through
     *  - Iterate only through the borders
     *     - If the minion is square, iteration through the borders only requires one loop
    j*/
    // check if minions are overlapping
    for (int j = m1->dim.y; j < m1->dim.y + m1->dim.h; j++) {
        for (int i = m1->dim.x; i < m1->dim.x + 2 * m1->dim.w; i++) {
            if (i >= m2->dim.x && i < m2->dim.x + 2 * m2->dim.w &&
                    j >= m2->dim.y && j < m2->dim.y + m2->dim.h)
                return OVERLAPPING;
        }
    }

    for (int i = m1->dim.x; i < m1->dim.x + 2 * m1->dim.w; i++) {
        if (i >= m2->dim.x && i < m2->dim.x + 2 * m2->dim.w) {
            if (m1->dim.y == m2->dim.y + m2->dim.h)
                return TOUCHING_N;

            if (m1->dim.y + m1->dim.h == m2->dim.y)
                return TOUCHING_S;
        }
    }

    for (int j = m1->dim.y; j < m1->dim.y + m1->dim.h; j++) {
        if (j >= m2->dim.y && j < m2->dim.y + m2->dim.h) {
            if (m1->dim.x == m2->dim.x + 2 * m2->dim.w)
                return TOUCHING_E;

            if (m1->dim.x + 2 * m1->dim.w == m2->dim.x)
                return TOUCHING_W;
        }
    }


    return APART;
}

void sk_minion_step(SKMinion *m)
{
    if (!m) {
        HE("invalid arguments", "sk_minion_move");
        return;
    }

    sk_minion_move(m, m->dim.x + 2 * m->speed.x, m->dim.y + m->speed.y);
}

#define MAX_MINIONS 10
typedef struct {
    SKMinion *minions[MAX_MINIONS];
    int num_minions;
    UIRect dim;
    char border;
} SKGru;

SKGru *sk_gru_new(UIRect dim, char border)
{
    SKGru *g = oopsalloc(1, sizeof(SKGru), "sk_gru_new");
    g->dim = dim;
    g->border = border;
    return g;
}

void sk_gru_destroy(SKGru *g)
{
    if (!g) {
        HE("invalid arguments", "sk_gru_destroy");
        return;
    }

    for (int i = 0; i < g->num_minions; sk_minion_destroy(g->minions[i++]));
    free(g);
}

void sk_gru_add_minion(SKGru *g, SKMinion *m)
{
    if (!g || !m) {
        HE("invalid arguments", "sk_gru_add_minion");
        return;
    }

    g->minions[g->num_minions++] = m;
}

void sk_gru_draw(SKGru *g)
{
    if (!g) {
        HE("invalid arguments", "sk_gru_draw");
        return;
    }

    // draw the frame
    _draw_rect(g->dim, g->border, ' ');

    // draw every minion
    for (int i = 0; i < g->num_minions; sk_minion_draw(g->minions[i++]));
}

void sk_gru_next_frame(SKGru *g)
{
    if (!g) {
        HE("invalid arguments", "sk_gru_draw");
        return;
    }

    for (int i = 0; i < g->num_minions; g->minions[i++]->is_speed_updated = false);

    for (int i = 0; i < g->num_minions; i++) {
        // check for collisions with other minions
        for (int j = 0; j < g->num_minions; j++) {
            SKMinionRelativePosition pos;
            if (g->minions[j]->is_speed_updated == false && g->minions[i] != g->minions[j]) {
                pos = sk_minion_relative_pos(g->minions[i], g->minions[j]);
                switch (pos) {
                    case TOUCHING_N:
                    case TOUCHING_S:
                        g->minions[j]->speed.y *= -1;
                        break;
                    case TOUCHING_W:
                    case TOUCHING_E:
                        g->minions[j]->speed.x *= -1;
                        break;
                    case OVERLAPPING:
                        g->minions[i]->speed.x *= -1;
                        g->minions[i]->speed.y *= -1;
                        break;
                    default:
                        continue;
                }
                g->minions[j]->is_speed_updated = true;
            }
        }

        // bounce on the gru frame
        if (g->minions[i]->dim.x <= g->dim.x + 2 ||
                g->minions[i]->dim.x + 2 * g->minions[i]->dim.w >= g->dim.x + 2 * g->dim.w - 2)
            g->minions[i]->speed.x *= -1;

        if (g->minions[i]->dim.y <= g->dim.y + 1 ||
                g->minions[i]->dim.y + g->minions[i]->dim.h >= g->dim.y + g->dim.h - 2)
            g->minions[i]->speed.y *= -1;

        sk_minion_step(g->minions[i]);
    }
}

void *enemies(void *s)
{
    SKGru *g = (SKGru *)s;
    while (1) {
        usleep(5e4);
        sk_gru_next_frame(g);
    }
    return NULL;
}

int main(void)
{
    term_setup(stdin, stdout);
    printf("\033[2J");

    FILE *mar = fopen("assets/img/resource_7.png", "r");
    Sprite *marabini = sprite_new(mar);
    fclose(mar);

    SKGru *g = sk_gru_new((UIRect) {2, 2, 300, 150}, 'M');
    SKMinion *m1 = sk_minion_new((UIRect) {55,  50, sprite_get_w(marabini), sprite_get_h(marabini)}, '1', '@', (SKVector) { -3, -2}, marabini);
    SKMinion *m2 = sk_minion_new((UIRect) {155, 50, sprite_get_w(marabini), sprite_get_h(marabini)}, '2', '@', (SKVector) { 2, -2}, marabini);
    SKMinion *m3 = sk_minion_new((UIRect) {205, 50, sprite_get_w(marabini), sprite_get_h(marabini)}, '3', '@', (SKVector) { -2, 3}, marabini);
    SKMinion *m4 = sk_minion_new((UIRect) {255, 50, sprite_get_w(marabini), sprite_get_h(marabini)}, '4', '@', (SKVector) { -2, -2}, marabini);
    SKMinion *m5 = sk_minion_new((UIRect) {305, 50, sprite_get_w(marabini), sprite_get_h(marabini)}, '5', '@', (SKVector) { 2, 2}, marabini);
    sk_gru_add_minion(g, m1);
    sk_gru_add_minion(g, m2);
    sk_gru_add_minion(g, m3);
    sk_gru_add_minion(g, m4);
    sk_gru_add_minion(g, m5);
    sk_gru_draw(g);

    pthread_t thr;
    pthread_create(&thr, NULL, enemies, g);

    /* struct timespec start, end; */
    /* clock_gettime(CLOCK_REALTIME, &start); */
    /* for (int i = 0; i < 1000; i++) */
    /*     sk_minion_relative_pos(m, n); */
    /* clock_gettime(CLOCK_REALTIME, &end); */
    /* long delta_usec = end.tv_nsec / 1000 - start.tv_nsec / 1000; */
    /* printf("time: %ld(us)\n", delta_usec); */

    int key;
    while ((key = term_read_key(stdin)) != 'q') {
        switch (key) {
            case 'n':
                sk_gru_next_frame(g);
                break;
            case UP_ARROW:
                break;
            case DOWN_ARROW:
                break;
            case LEFT_ARROW:
                break;
            case RIGHT_ARROW:
                break;
            default:
                break;
        }
    }
    sk_gru_destroy(g);
    term_teardown(stdin, stdout);
    return 0;
}
