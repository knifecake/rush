#include "minitest.h"

#include "../src/lib/config.h"
#include "../src/entities/world.h"

#define TEST_CONFIG_FILE "test/assets/config.txt"

int main(void) {
    load_config_from_file(TEST_CONFIG_FILE);

    World *w = world_new();
    assert("can create world", w);

    int *wallet= world_get_wallet(w);
    bool truth=true;

    for(int i=0; i<MAX_RESOURCES; i++)
      truth*=(wallet[i]==0);

    assert("wallet is completely empty at the start", truth==true);

    world_destroy(w);
    assert("can destroy world", true);

    return failed_tests();
}
