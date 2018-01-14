#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "src/ui.h"
#include "src/lib/sprite.h"
#include "src/lib/terminal.h"

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

    FILE *gin = fopen("assets/img/resource_6.png", "r");
    Sprite *ginebra = sprite_new(gin);
    fclose(gin);

    SKGru *g = sk_gru_new((UIRect) {2, 2, 300, 150}, 'M');
    SKMinion *m1 = sk_minion_new((UIRect) {55,  50, sprite_get_w(marabini), sprite_get_h(marabini)}, '1', '@', (SKVector) { -3, -2}, marabini);
    SKMinion *m2 = sk_minion_new((UIRect) {155, 50, sprite_get_w(marabini), sprite_get_h(marabini)}, '2', '@', (SKVector) { 2, -2}, marabini);
    SKMinion *m3 = sk_minion_new((UIRect) {205, 50, sprite_get_w(marabini), sprite_get_h(marabini)}, '3', '@', (SKVector) { -2, 3}, marabini);
    SKMinion *m4 = sk_minion_new((UIRect) {255, 50, sprite_get_w(marabini), sprite_get_h(marabini)}, '4', '@', (SKVector) { -2, -2}, marabini);
    SKMinion *m5 = sk_minion_new((UIRect) {301, 50, sprite_get_w(marabini), sprite_get_h(marabini)}, '5', '@', (SKVector) { 2, 2}, marabini);

    SKMinion *p = sk_minion_new((UIRect) { 401, 51, sprite_get_w(ginebra), sprite_get_h(ginebra) }, 'P', 'P', (SKVector) { 0 }, ginebra);
    sk_gru_add_minion(g, m1);
    sk_gru_add_minion(g, m2);
    sk_gru_add_minion(g, m3);
    sk_gru_add_minion(g, m4);
    sk_gru_add_minion(g, m5);
    sk_gru_add_minion(g, p);
    sk_gru_draw(g);

    pthread_t thr;
    pthread_create(&thr, NULL, enemies, g);

    int key;
    while ((key = term_read_key(stdin)) != 'q') {
        switch (key) {
            case 'n':
                sk_gru_next_frame(g);
                break;
            case UP_ARROW:
                sk_minion_update_position(p, 0, -2);
                break;
            case DOWN_ARROW:
                sk_minion_update_position(p, 0, 2);
                break;
            case LEFT_ARROW:
                sk_minion_update_position(p, -4, 0);
                break;
            case RIGHT_ARROW:
                sk_minion_update_position(p, 4, 0);
                break;
            default:
                break;
        }
    }
    sk_gru_destroy(g);
    term_teardown(stdin, stdout);
    return 0;
}
