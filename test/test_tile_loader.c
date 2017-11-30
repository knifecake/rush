#include "minitest.h"

#include "../src/entities/tile.h"
#include "../src/asset_loaders/tile_loader.h"

#define VALID_TILE_DB "test/assets/tiles.txt"
#define VALID_TILE_DB_LEN 2
#define LOADED_RESOURCES 2

#define INVALID_TILE_DB "test/assets/tiles-2.txt"

int main(void)
{
    FILE *db = fopen(VALID_TILE_DB, "r");
    FILE *db2 = fopen(INVALID_TILE_DB, "r");

    if (!db || !db2) {
        assert("could not open stub tile databases", false);
        return failed_tests();
    }

    Tile **tiles;
    assert("can load a valid tile db", tiles = load_tiles_from_file(db, LOADED_RESOURCES));

    assert("can count the number of tiles in an array", entity_list_len(tiles) == VALID_TILE_DB_LEN);

    tile_list_destroy(tiles);

    assert("refuses to load a file containing too many resources",
            !load_tiles_from_file(db2, LOADED_RESOURCES));

    return failed_tests();
}
