
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../../lib/lineread.h"

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

    // holds an array of copies of canonical tiles created randomly on startup
    Tile **map;
    int map_tiles;

    // keeps a canonical copy of each type of tile
    Tile **tiles;
    int num_tiles;

    Building **buildings;
    int num_buildings;

    Event **events;
    int num_events;
    int level;
};

/*
int _aleat_num (int inf, int sup){
  int temp, r;
  if (inf > sup){
    temp = inf;
    inf = sup;
    sup = temp;
  }
  sup++;
  r = (int) inf + rand() / (RAND_MAX / (sup - inf) + 1);
  return r;
}
*/

int aleat_num(int inf, int sup){
  int n, r;
  long end;

  if(inf<sup){
    n=sup-inf+1;
  }
  else if(sup<inf){
    n=inf-sup+1;
  }
  else{
    fprintf(stderr, "aleat_num: cant generate a random number between the same number\n");
    return -1;
  }

  if ((n - 1) == RAND_MAX) {
      return rand();
  }
  else {
      /* Chop off all of the values that would cause skew...*/

      end = RAND_MAX / n; /* truncate skew*/
      assert (end > 0L);
      end *= n;

      /* ... and ignore results from rand() that fall above that limit.
       (Worst case the loop condition should succeed 50% of the time,
       so we can expect to bail out of this loop pretty quickly.)*/

      while ((r = rand()) >= end);

      r = r % n;
      if(inf<sup){
        r=inf+r;
      }
      else{
        r=sup+r;
      }
      return r;
    }

}

int _world_load_game_state(World *w, char *game_state_file)
{
    if (!w || !game_state_file) {
        HE("invalid arguments", "_world_load_game_state");
        return UINT_ERROR;
    }

    FILE *gs = fopen(game_state_file, "r");
    if (!gs) {
        HE("cannot open game state file", "_world_load_game_state");
        return UINT_ERROR;
    }

    // read number of resources
    char *buff = fgetll(gs);
    if (!buff) {
        HE("could not read number of resources", "_world_load_game_state");
        return UINT_ERROR;
    }
    int nres = atoi(buff); free(buff);
    if (nres != w->num_resources) {
        HE("refusing to load game state with different number of resources than are defined", "_world_load_game_state");
        return UINT_ERROR;
    }

    for (int i = 0; i < w->num_resources; i++) {
        // read the quentity of the i-th resource
        buff = fgetll(gs);
        if (!buff) {
            HE("could not read quantity of a resource", "_world_load_game_state");
            return UINT_ERROR;
        }

        // add it to our wallet
        w->wallet[i] = atoi(buff); free(buff);
    }

    fclose(gs);

    return !UINT_ERROR;
}

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

    w->tiles = load_tiles_from_file(tf, w->num_resources);
    if (!w->tiles) {
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

    // load events
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
    for (w->num_tiles = 0; w->tiles[w->num_tiles]; w->num_tiles++);
    for (w->num_buildings = 0; w->buildings[w->num_buildings]; w->num_buildings++);
    for (w->num_events = 0; w->events[w->num_events]; w->num_events++);

    // create the map
    int height  = config_get_int("map height");
    int columns = config_get_int("map columns");
    w->map_tiles = height * columns;

    // as always, list of entities are null terminated
    w->map = oopsalloc(w->map_tiles + 1, sizeof(Tile *),"world_new");

    // choose a tile type at random from the list of tiles for each cell in the map
    for (int i=0; i < w->map_tiles; i++){
      //w->map[i] = tile_copy(w->tiles[_aleat_num(0, w->num_tiles-1)]);
      w->map[i] = tile_copy(w->tiles[aleat_num(0, w->num_tiles - 1)]);
    }

    // load initial game state
    char *initial_game_state = config_get("initial game state");
    if (!initial_game_state) {
        HE("don't know where to load initial game state from, not loading anything", "world_new");
    } else {
        if (UINT_ERROR == _world_load_game_state(w, initial_game_state)) {
            HE("initial game state could not be loaded", "world_new");
        }
    }

    return w;
}

void world_destroy(World *w) {
  if(!w){
    HE("invalid parameters", "world_destroy");
    return;
  }

  building_list_destroy(w->buildings);
  tile_list_destroy(w->map);
  tile_list_destroy(w->tiles);
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
  /*
  / Collect all the resources from the tiles with resources buildings and reduce by one
  / the turns of the active events
  */

  for(int i = 0; i < w->map_tiles; i++){
    int *resources = (int *) oopsalloc(MAX_RESOURCES, sizeof(int), "world_next_turn");
    w->map[i] = tile_next_turn(w->map[i], resources);
    for(int j = 0; j < w->num_resources; j++){
      w->wallet[j] += resources[j];
    }
    free(resources);
  }

  /*
  / TODO: See how many events we want to manage with and modify this
  / so that the probabilties are consistent with that
  */
 int i = 0;
  while(i < w->num_tiles){
    int tile_affected = aleat_num(0, w->num_tiles);
    int affecting_event = aleat_num(0, w->num_events);
    tile_set_event(w->tiles[tile_affected], w->events[affecting_event]);
    i += aleat_num(0, w->num_tiles);
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

  return w->tiles;
}

Tile **world_get_map(World *w)
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

int world_wallet_delta(World *w, int resource_id, int delta)
{
    if (!w || resource_id < 0 || resource_id >= w->num_resources) {
        HE("invalid arguments", "world_wallet_delta");
        return UINT_ERROR;
    }

    w->wallet[resource_id] += delta;

    return UINT_ERROR;
}

Tile *world_tile_at_index(World *w, int tile_index)
{
    if (!w || tile_index < 0 || tile_index >= w->map_tiles) {
        HE("invalid arguments", "world_tile_at_index");
        return NULL;
    }

    return w->map[tile_index];
}

int world_build_on_tile(World *w, int tile_index, Building *b)
{
    if (!w || tile_index < 0 || tile_index >= w->map_tiles || !b) {
        HE("invalid arguments", "world_build_on_tile");
        return UINT_ERROR;
    }

    // check if building can be build at the player's current level
    int unlock_level = building_get_unlocking_level(b);
    if (unlock_level > w->level)
        return WORLD_BUILD_NO_LEVEL;

    // check the player has enough money
    int cost = building_get_cost(b);
    if (cost > w->wallet[COST_RESOURCE])
        return WORLD_BUILD_NO_MONEY;

    // link building to tile
    if (UINT_ERROR == tile_build(w->map[tile_index], b)) {
        HE("could not build for some reason", "world_build_on_tile");
        return UINT_ERROR;
    }

    // substract money from wallet
    w->wallet[COST_RESOURCE] -= cost;

    // if building is a town hall, increase level
    if (building_is_townhall(b)) {
        w->level = building_get_level(b);
        return WORLD_BUILD_SUCCESS_LEVEL_UP;
    }

    return WORLD_BUILD_SUCCESS;
}
