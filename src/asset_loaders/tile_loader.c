#include "tile_loader.h"

#include "../lib/error_handling.h"
#include "../../lib/lineread.h"

#include <stdlib.h>


Tile **load_tiles_from_file(FILE *f, int num_resources)
{
    if (!f) {
        HE("invalid parameters (no file given)", "load_tiles_from_file")
        return NULL;
    }

    int num_tiles = 0;
    char *buff = fgetll(f);
    if (!buff) {
        HE("cannot determine the number of tiles to load", "load_tiles_from_file")
        return NULL;
    }
    num_tiles = atoi(buff); free(buff);
    if (num_tiles == 0) {
        HE("refusing to load tiles from an empty or corrupt file", "load_tiles_from_file");
        return NULL;
    }

    // we'll leave the last pointer set to null, to signal the end of the list
    Tile **tiles = calloc(num_tiles + 1, sizeof(Tile *));
    if (!tiles) {
        HE("cannot load tiles, out of memory", "load_tiles_from_file")
        return NULL;
    }

    for (int i = 0; i < num_tiles; i++) {
        buff = fgetll(f);
        int id = atoi(buff); free(buff);

        char *sprite = fgetll(f);
        if (!sprite) {
            HE("could not retrieve sprite for a tile", "load_tiles_from_file")
            tile_list_destroy(tiles); return NULL;
        }

        // load resource multipliers
        float *resource_multipliers = calloc(MAX_RESOURCES, sizeof(float));
        if (!resource_multipliers) {
            HE("cannot load tile from file, out of memory", "load_tiles_from_file")
            free(sprite); tile_list_destroy(tiles); return NULL;
        }
        for (int j = 0; j < num_resources; j++) {
            buff = fgetll(f);
            if (!buff) {
                HE("error reading resource multipliers for a tile", "load_tiles_from_file")
                free(sprite); free(resource_multipliers); tile_list_destroy(tiles); return NULL;
            }
            resource_multipliers[j] = atof(buff); free(buff);
        }

        int *available_resources = calloc(MAX_RESOURCES, sizeof(float));
        if (!available_resources) {
            HE("cannot load tile from file, out of memory", "load_tiles_from_file")
            free(sprite); free(resource_multipliers); tile_list_destroy(tiles); return NULL;
        }
        for (int j = 0; j < num_resources; j++) {
            buff = fgetll(f);
            if (!buff) {
                HE("error reading available resources for a tile", "load_tiles_from_file")
                free(sprite); free(resource_multipliers); free(available_resources);
                tile_list_destroy(tiles); return NULL;
            }
            available_resources[j] = atoi(buff); free(buff);
        }

        buff = fgetll(f);
        if (!buff) {
            HE("error reading enemies for a tile", "load_tiles_from_file")
            free(sprite); free(resource_multipliers); free(available_resources);
            tile_list_destroy(tiles); return NULL;
        }
        int enemies = atoi(buff); free(buff);

        tiles[i] = tile_new(id, sprite, resource_multipliers, available_resources, enemies);
        free(sprite); free(resource_multipliers); free(available_resources);
        if (!tiles[i]) {
            HE("could not create tile", "load_tiles_from_file")
            tile_list_destroy(tiles); return NULL;
        }
    }

    return tiles;
}


void tile_list_destroy(Tile **list)
{
    if (!list) return;

    for (int i = 0; list[i]; tile_destroy(list[i++]));
    free(list);
}
