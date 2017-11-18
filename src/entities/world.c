
#include <stdio.h>
#include <stdlib.h>

#include "world.h"
#include "../lib/error_handling.h"
#include "../lib/sprite.h"

#include "../asset_loaders/resource_loader.h"
#include "../asset_loaders/tile_loader.h"

/*
 * Provisional constant.
 *
 * Describes where are we loading our resources from relative to the directory
 * from where the executable file will be called. See Makefile or README.md for
 * information on where this file is.
 */
#define RESOURCES_DB "assets/resources.txt"
#define TILES_DB "assets/tiles.txt"
#define EVENTS_DB "assets/events.txt"
#define IMAGE_ASSET "assets/img/GUI.txt"

struct _World {
    Resource **resources;
    int num_resources;

    Tile **map;
    int current_tile;

    Building **b;
};

World *world_new(void) {

    World *w = NULL;
    w = (World *) calloc(1, sizeof(World));
    if (!w) {
        HE("malloc error","world_new");
        return NULL;
    }

    FILE *rf = fopen(RESOURCES_DB, "r");
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

    FILE *tf = fopen(TILES_DB, "r");
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

    return w;
}

void world_destroy(World *w) {
  if(!w){
    HE("invalid parameters", "world_destroy");
    return;
  }

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

void world_print(FILE *s, World *w)
{
    if (!s || !w) {
        HE("invalid parameters", "world_print");
    }

    fprintf(s, "World at %p:\n", w);
    fprintf(s, "Resources (%d):\n", w->num_resources);
    for (int i = 0; i < w->num_resources; resource_print(s, w->resources[i++]));

    fprintf(s, "\nTiles (current is at index %d):\n", w->current_tile);
    for (int i = 0; w->map[i]; tile_print(s, w->map[i++]));
}
