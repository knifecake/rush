#include "minitest.h"

#include "../src/lib/sprite.h"

#define VALID_SPRITE "test/assets/valid-sprite.png"
#define VALID_SPRITE_W 32
#define VALID_SPRITE_H 32

int main(void) {

    FILE *f = fopen(VALID_SPRITE, "r");
    if (!f) {
        assert("could not find stub sprite for testing", false);
        return failed_tests();
    }

    Sprite *s;
    assert("can load a png from a valid file", s = sprite_new(f));

    assert("can return the width of a sprite", sprite_get_w(s) == VALID_SPRITE_W);
    assert("can return the height of a sprite", sprite_get_h(s) == VALID_SPRITE_H);

    return failed_tests();
}
