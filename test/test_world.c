#include <limits.h>

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

    Building **buildings = world_get_buildings(w);

    int estado = world_build_on_tile(w, 5, buildings[0]);

    assert("can't build in the tiles without resources", estado==2);

    for(int i = 0; i < MAX_RESOURCES; i++){
      world_wallet_delta(w, i, INT_MAX);
    }
    wallet = world_get_wallet(w);

    for(int i=0; i<MAX_RESOURCES; i++)
      truth*=(wallet[i]==INT_MAX);

    assert("world_wallet_delta runs correctly", truth==true);

    estado = world_build_on_tile(w, 5, buildings[0]);

    assert("can build in the tiles with enough resources", estado==4);

    estado = world_build_on_tile(w, 5, buildings[0]);

    assert("can't build in the tiles that already has a building", estado==6);

    world_destroy(w);
    assert("can destroy world", true);

    return failed_tests();
}
