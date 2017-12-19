
#include <stdio.h>
#include <stdlib.h>

#include "world.h"

#include "../lib/error_handling.h"
#include "../lib/terminal.h"
#include "../lib/messages.h"
#include "../lib/config.h"

#include "../asset_loaders/resource_loader.h"
#include "../asset_loaders/tile_loader.h"
#include "../asset_loaders/building_loader.h"
#include "../asset_loaders/event_loader.h"

struct _World {
    Resource **resources;
	  int wallet[MAX_RESOURCES];
    int num_resources;

    Tile **map;
    int cursor;
    int num_tiles;

    Building **buildings;
    int num_buildings;

    Event **events;
    int num_events;
};

World *world_new(void) {

    World *w = NULL;
    w = oopsalloc(1, sizeof(World), "world_new");

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

    w->resources = oopsalloc(MAX_RESOURCES, sizeof(Resource *), "world_new");

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

    // LOAD EVENTS
    char *events_db = config_get("events db");
    if (!events_db) {
        HE("don't know where to load events from, set up an 'event db' entry in config", "world_new");
        resource_list_destroy(w->resources, w->num_resources);
        tile_list_destroy(w->map);
        building_list_destroy(w->buildings); free(w);
        return NULL;
    }

    FILE *ef = fopen(events_db, "r");
    if (!bf) {
        HE("could not open buildings db file", "world_new");
        resource_list_destroy(w->resources, w->num_resources);
        tile_list_destroy(w->map);
        building_list_destroy(w->buildings); free(w);
        return NULL;
    }

    w->events = load_events_from_file(bf, w->num_resources);
    if (!w->events) {
        HE("could not load events", "world_new");
        resource_list_destroy(w->resources, w->num_resources);
        tile_list_destroy(w->map);
        building_list_destroy(w->buildings); free(w);
        return NULL;
    }
    fclose(ef);

    // count tiles, buildings and events
    for (w->num_tiles = 0; w->map[w->num_tiles]; w->num_tiles++);
    for (w->num_buildings = 0; w->buildings[w->num_buildings]; w->num_buildings++);
    for (w->num_events = 0; w->events[w->num_events]; w->num_events++);

     return w;
}

void world_destroy(World *w) {
  if(!w){
    HE("invalid parameters", "world_destroy");
    return;
  }

  building_list_destroy(w->buildings);
  tile_list_destroy(w->map);
  event_list_destroy(w->events);
  resource_list_destroy(w->resources, w->num_resources);
  free(w);
}

//Maybe we need to call some other functions like create events
World *world_next_turn(World *w){
  if(!w){
    HE("invalid parameters", "world_player_turn");
    return NULL;
  }

  for(int i = 0; i < w->num_tiles; i++){
    int *resources = (int *) oopsalloc(MAX_RESOURCES, sizeof(int), "world_next_turn");
    w->map[i] = tile_next_turn(w->map[i], resources);
    for(int j = 0; j < w->num_resources; j++){
      w->wallet[j] += resources[j];
    }
    free(resources);
  }
  return w;
}

int *world_get_wallet(World *w)
{
  if(!w) {
    HE("invalid parameters", "world_get_wallet");
    return NULL;
  }

  return w->wallet;
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
                w->cursor++;
            else
                show_msg("cannot move further down\n");
            break;
        case UP_ARROW:
            if (w->cursor > 0)
                w->cursor--;
            else
                show_msg("cannot move further up\n");
            break;
        default:
            show_msg("that direction is not yet supported\n");
    }

    return w;
}

int world_get_cursor(World *w)
{
    if (!w) {
        HE("invalid arguments", "world_get_cursor");
        return UINT_ERROR;
    }

    return w->cursor;
}

Tile *world_get_current_tile(World *w)
{
    if (!w) {
        HE("invalid arguments", "world_get_current_tile");
        return NULL;
    }

    return w->map[w->cursor];
}

Building **world_get_buildings(World *w)
{
  if (!w) {
      HE("invalid parameters", "world_get_buildings");
      return NULL;
  }

  return w->buildings;
}


int world_get_num_buildings(World *w)
{
  if (!w) {
      HE("invalid parameters", "world_get_num_buildings");
      return UINT_ERROR;
  }

  return w->num_buildings;
}

Tile **world_get_tiles(World *w)
{
  if (!w) {
      HE("invalid parameters", "world_get_tiles");
      return NULL;
  }

  return w->map;
}


int world_get_num_tiles(World *w)
{
  if (!w) {
      HE("invalid parameters", "world_get_num_tiles");
      return UINT_ERROR;
  }

  return w->num_tiles;
}

Resource **world_get_resources(World *w)
{
    if (!w) {
        HE("invalid arguments", "world-get_resources");
        return NULL;
    }

    return w->resources;
}

int world_get_resource_quantity(World *w, int resource_id)
{
    if (!w || resource_id >= w->num_resources) {
        HE("invalid arguments", "world_get_resource_quantity");
        return UINT_ERROR;
    }

    return w->wallet[resource_id];
}

int world_get_num_resources(World *w)
{
    if (!w) {
        HE("invalid arguments", "world_get_num_resources");
        return UINT_ERROR;
    }

    return w->num_resources;
}

Event **world_get_events(World *w)
{
  if (!w) {
      HE("invalid parameters", "world_get_events");
      return NULL;
  }

  return w->events;
}


int world_get_num_events(World *w)
{
  if (!w) {
      HE("invalid parameters", "world_get_num_events");
      return UINT_ERROR;
  }

  return w->num_events;
}

void world_print(FILE *s, World *w)
{
    if (!s || !w) {
        HE("invalid parameters", "world_print");
	return;
    }

    fprintf(s, "World at %p:\n", w);
    fprintf(s, "Resources (%d):\n", w->num_resources);
    for (int i = 0; i < w->num_resources; resource_print(s, w->resources[i++]));

    fprintf(s, "\nTiles:\n");
    for (int i = 0; w->map[i]; tile_print(s, w->map[i++]));

    fprintf(s, "\nBuildings:\n");
    for (int i = 0; w->buildings[i]; building_print(s, w->buildings[i++]));
}
