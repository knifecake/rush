#include "minitest.h"

#include "../src/lib/config.h"
#include "../src/entities/world.h"

#define TEST_CONFIG_FILE "test/assets/config.txt"

int main(void) {
    load_config_from_file(TEST_CONFIG_FILE);

    World *w = world_new();
    assert("can create world", w);

    world_destroy(w);
    assert("can destroy world", true);

    return failed_tests();
}
