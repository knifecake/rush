
#include <stdio.h>
#include <stdlib.h>

#include "world.h"

#include "../lib/error_handling.h"
#include "../lib/game_controller.h"
#include "../lib/messages.h"
#include "../lib/config.h"

#include "../asset_loaders/resource_loader.h"
#include "../asset_loaders/tile_loader.h"
#include "../asset_loaders/building_loader.h"

struct _World {
    Resource **resources;
    int num_resources;

    Tile **map;
    int cursor;
    int num_tiles;

    Building **buildings;
    int num_buildings;
};

World *world_new(void) {

    World *w = NULL;
    w = (World *) calloc(1, sizeof(World));
    if (!w) {
        HE("malloc error","world_new");
        return NULL;
    }

    // LOAD RESOURCES
    char *resources_db = config_get("resources db");
    if (!resources_db) {
        HE("don't know where to load resources from, set up a 'resources db' entry in config", "world_new");
        free(w); return NULL;
    }

    FILE *rf = fopen(resources_db, "r");
    if (!rf) {
        HE("could not open resources db file", "world_new")
        free(w);
        return NULL;
    }

    w->resources = malloc(MAX_RESOURCES * sizeof(Resource *));
    if (!w->resources) {
        HE("could not allocate memory for resources", "world_new");
        free(w);
        return NULL;
    }

    w->num_resources = load_resources_from_file(rf, w->resources);
    if (w->num_resources == 0) {
        HE("could not load resources", "world_new")
        resource_list_destroy(w->resources, w->num_resources); free(w);
        return NULL;
    }

    fclose(rf);

    // LOAD TILES
    char *tiles_db = config_get("tiles db");
    if (!tiles_db) {
        HE("don't know where to load tiles from, set up a 'tiles db' entry in config", "world_new");
        resource_list_destroy(w->resources, w->num_resources); free(w);
        return NULL;
    }

    FILE *tf = fopen(tiles_db, "r");
    if (!tf) {
        HE("could not open tiles db file", "world_new");
        resource_list_destroy(w->resources, w->num_resources); free(w);
        return NULL;
    }

    w->map = load_tiles_from_file(tf, w->num_resources);
    if (!w->map) {
        HE("could not load tiles", "world_new");
        resource_list_destroy(w->resources, w->num_resources);
        free(w);
        return NULL;
    }
    fclose(tf);

    // LOAD BUILDINGS
    char *buildings_db = config_get("buildings db");
    if (!buildings_db) {
        HE("don't know where to load buildings from, set up a 'buildings db' entry in config", "world_new");
        resource_list_destroy(w->resources, w->num_resources);
        tile_list_destroy(w->map); free(w);
        return NULL;
    }

    FILE *bf = fopen(buildings_db, "r");
    if (!bf) {
        HE("could not open buildings db file", "world_new");
        resource_list_destroy(w->resources, w->num_resources);
        tile_list_destroy(w->map); free(w);
        return NULL;
    }

    w->buildings = load_buildings_from_file(bf, w->num_resources);
    if (!w->buildings) {
        HE("could not load tiles", "world_new");
        resource_list_destroy(w->resources, w->num_resources);
        tile_list_destroy(w->map); free(w);
        return NULL;
    }
    fclose(bf);

    // count tiles and buildings
    for (w->num_tiles = 0; w->map[w->num_tiles]; w->num_tiles++);
    for (w->num_buildings = 0; w->buildings[w->num_buildings]; w->num_buildings++);

    return w;
}

void world_destroy(World *w) {
  if(!w){
    HE("invalid parameters", "world_destroy");
    return;
  }

  building_list_destroy(w->buildings);
  tile_list_destroy(w->map);
  resource_list_destroy(w->resources, w->num_resources);
  free(w);
}

World *world_player_turn(World *w) {
  if(!w){
    HE("invalid parameters", "world_player_turn");
    return NULL;
  }
  /*



  */
  return w;
}

World *world_ai_turn(World *w){
  if(!w){
    HE("invalid parameters", "world_player_turn");
    return NULL;
  }
  /*



  */
  return w;
}

World *world_move_cursor(World *w, int dir)
{
    if (!w) {
        HE("invalid parameters", "world_move_cursor");
        return NULL;
    }

    switch (dir) {
        case DOWN_ARROW:
            if (w->cursor < w->num_tiles - 1)
                show_msg("you moved down, now at tile %d\n", ++w->cursor);
            else
                show_msg("cannot move further down\n");
            break;
        case UP_ARROW:
            if (w->cursor > 0)
                show_msg("you moved up, now at tile %d\n", --w->cursor);
            else
                show_msg("cannot move further up\n");
            break;
        default:
            show_msg("that direction is not yet supported\n");
    }

    return w;
}

void world_print(FILE *s, World *w)
{
    if (!s || !w) {
        HE("invalid parameters", "world_print");
    }

    fprintf(s, "World at %p:\n", w);
    fprintf(s, "Resources (%d):\n", w->num_resources);
    for (int i = 0; i < w->num_resources; resource_print(s, w->resources[i++]));

    fprintf(s, "\nTiles (current is at index %d):\n", w->cursor);
    for (int i = 0; w->map[i]; tile_print(s, w->map[i++]));
}
