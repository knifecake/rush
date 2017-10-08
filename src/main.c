#include <stdio.h>

#include "../lib/lineread.h"

#include "error_handling.h"

#include "asset_loaders/resource_loader.h"
#include "asset_loaders/tile_loader.h"

#include "sprite.h"


/*
 * Provisional constant.
 *
 * Describes where are we loading our resources from relative to the directory
 * from where the executable file will be called. See Makefile or README.md for
 * information on where this file is.
 */
#define RESOURCES_DB "assets/resources.txt"
#define TILES_DB "assets/tiles.txt"
#define IMAGE_ASSET "assets/img/color_key.bmp"


int main(void) {
    printf("Loading resources...\n");
    FILE *rf = fopen(RESOURCES_DB, "r");
    if (!rf) {
        handle_error("could not open resources db file", "main", __FILE__, __LINE__);
        return PINT_ERROR;
    }

    Resource **resources = malloc(MAX_RESOURCES * sizeof(Resource *));
    int num_resources = load_resources_from_file(rf, resources);
    if (num_resources == 0) {
        handle_error("could not load resources", "main", __FILE__, __LINE__);
        return PINT_ERROR;
    }

    fclose(rf);

    for (int i = 0; i < num_resources; resource_print(stdout, resources[i++]));

    resource_list_destroy(resources, num_resources);


    printf("\nLoading tiles...\n");
    FILE *tf = fopen(TILES_DB, "r");
    if (!tf) {
        handle_error("could not open tiles db file", "main", __FILE__, __LINE__);
        return PINT_ERROR;
    }

    Tile **tiles = load_tiles_from_file(tf, num_resources);
    if (!tiles) {
        handle_error("could not load tiles", "main", __FILE__, __LINE__);
        return PINT_ERROR;
    }

    fclose(tf);

    for (int i = 0; tiles[i]; tile_print(stdout, tiles[i++]));

    FILE *imf = fopen(IMAGE_ASSET, "r");
    Sprite *s = sprite_new(imf);
    sprite_draw(s, 1, 1);
    sprite_destroy(s);
    fclose(imf);

    tile_list_destroy(tiles);

    return 0;
}
