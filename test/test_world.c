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
    int nresources = world_get_num_resources(w);

    for(int i=0; i<nresources; i++)
      truth*=(wallet[i]==0);

    assert("wallet is completely empty at the start", truth==true);

    Building **buildings = world_get_buildings(w);

    int estado = world_build_on_tile(w, 2, buildings[2]);

    assert("can't build in the tiles without enough resources", estado==2);

    for(int i = 0; i < nresources; i++){
      world_wallet_delta(w, i, 10000);
    }
    wallet = world_get_wallet(w);


    for(int i=0; i < nresources; i++)
      truth*=(wallet[i]==10000);

    assert("world_wallet_delta runs correctly", truth==true);

    estado = world_build_on_tile(w, 2, buildings[2]);

    assert("can build in the tiles with enough resources", estado==4);

    estado = world_build_on_tile(w, 2, buildings[2]);

    Tile **tiles = world_get_map(w);
    Building *edificio = tile_get_building(tiles[2]);
    /* building_print(stderr, edificio); */
    /* building_print(stderr, buildings[2]); */

    assert("can't build in the tiles that already has a building", estado==6);
    fprintf(stderr, "%d\n", estado);

    world_destroy(w);
    assert("can destroy world", true);

    return failed_tests();
}
