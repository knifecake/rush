#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "src/lib/terminal.h"
#include "src/lib/error_handling.h"

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
    bool is_drawn;
} SKMinion;

SKMinion *sk_minion_new(UIRect dim, char border, char fill, SKVector speed)
{
    SKMinion *m = oopsalloc(1, sizeof(SKMinion), "sk_minion_new");
    m->dim = dim;
    m->speed = speed;
    m->border = border;
    m->fill = fill;
    m->is_drawn = false;
    return m;
}

void sk_minion_destroy(SKMinion *m)
{
    free(m);
}

void sk_minion_draw(SKMinion *m)
{
    m->is_drawn = true;
    _draw_rect(m->dim, m->border, m->fill);
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

typedef enum { OVERLAPPING, APART, ARGUMENT_ERROR } SKMinionRelativePosition;

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
            if (i >= m2->dim.x && i <= m2->dim.x + 2 * m2->dim.w &&
                    j >= m2->dim.y && j <= m2->dim.y + m2->dim.h)
                return OVERLAPPING;
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
} SKGru;

SKGru *sk_gru_new()
{
    return oopsalloc(1, sizeof(SKGru), "sk_gru_new");
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

    for (int i = 0; i < g->num_minions; sk_minion_draw(g->minions[i++]));
}

void sk_gru_next_frame(SKGru *g)
{
    if (!g) {
        HE("invalid arguments", "sk_gru_draw");
        return;
    }

    for (int i = 0; i < g->num_minions; i++) {
        for (int j = 0; j < g->num_minions; j++) {
            if (g->minions[i] != g->minions[j] &&
                    sk_minion_relative_pos(g->minions[i], g->minions[j]) == OVERLAPPING) {
                g->minions[i]->speed.x = -g->minions[i]->speed.x;
                g->minions[i]->speed.y = -g->minions[i]->speed.y;
                g->minions[j]->speed.x = -g->minions[j]->speed.x;
                g->minions[j]->speed.y = -g->minions[j]->speed.y;
            }
        }
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

    SKGru *g = sk_gru_new();
    SKMinion *m = sk_minion_new((UIRect) {1, 1, 40, 4}, 'M', 'M', (SKVector) {0, 0});
    SKMinion *a = sk_minion_new((UIRect) {1, 5, 4, 16}, 'M', 'M', (SKVector) {0, 0});
    SKMinion *o = sk_minion_new((UIRect) {1, 20, 40, 4}, 'M', 'M', (SKVector) {0, 0});
    SKMinion *b = sk_minion_new((UIRect) {73, 5, 4, 16}, 'M', 'M', (SKVector) {0, 0});
    SKMinion *c = sk_minion_new((UIRect) {25, 6, 4, 4}, 'M', 'M', (SKVector) {0, 0});
    SKMinion *n = sk_minion_new((UIRect) {40, 15, 1, 2}, '+', 'x', (SKVector) {1, 1});
    sk_gru_add_minion(g, m);
    sk_gru_add_minion(g, a);
    sk_gru_add_minion(g, b);
    sk_gru_add_minion(g, c);
    sk_gru_add_minion(g, n);
    sk_gru_add_minion(g, o);
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

    while (term_read_key(stdin) != 'q');
    sk_gru_destroy(g);
    term_teardown(stdin, stdout);
    return 0;
}
