/*
 * PPROG Game – Morzilla Firefox 2018
 *
 * Authors: Miguel Baquedano, Sergio Cordero, Elias Hernandis
 *          and Rafael Sánchez.
 *
 * Lead author: <replace me>
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "../../lib/lineread.h"
#include "../../lib/qrnd.h"

#include "world.h"

#include "../lib/error_handling.h"
#include "../lib/terminal.h"
#include "../lib/messages.h"
#include "../lib/config.h"

#include "../asset_loaders/resource_loader.h"
#include "../asset_loaders/tile_loader.h"
#include "../asset_loaders/building_loader.h"
#include "../asset_loaders/event_loader.h"

#define N_SKILLS 4
#define MARAB_PROP 1.0
#define EAT_SKILL_PROP 0.5
#define EAT_SKILL_ID 0

struct _World {
    Resource **resources;
    int wallet[MAX_RESOURCES];
    int num_resources;

    int turn;

    // keeps a canonical copy of each type of tile
    Tile **tiles;
    int num_tiles;

    Map *map;
    int map_tiles;

    Building **buildings;
    int num_buildings;

    Event **events;
    int num_events;

    // player level
    int level;

    // randomness source
    rnd_state *rs;

    // only one townhall at most
    int n_townhalls;

    //save the index tile of the town_hall in the map
    int town_hall_index;

    // the timestamp of when the last turn happened
    time_t last_turn_timestamp;

    bool skills[N_SKILLS];
};

//giving an id it returns a copy of the canonic tile with that id
Tile *_world_canonical_tile_copy(World *w, int id)
{
  if(w==NULL || id < 0){
    return NULL;
  }
  int j = 0;
  while(j < w->num_tiles && id != tile_get_id(w->tiles[j])){
    j++;
  }
  return tile_copy(w->tiles[j]);
}

//giving an id it returns a copy of the canonic building with that id
Building *_world_canonical_building_copy(World *w, int id)
{
  if(w==NULL || id < 0){
    return NULL;
  }
  int j = 0;
  while(j < w->num_buildings && id != building_get_id(w->buildings[j])){
      j++;
  }
  return building_copy(w->buildings[j]);
}

//giving an id it returns a copy of the canonic event with that id
Event *_world_canonical_event_copy(World *w, int id)
{
  if(w==NULL || id < 0){
    return NULL;
  }
  int j = 0;
  while(j < w->num_events && id != event_get_id(w->events[j])){
      j++;
  }
  return event_copy(w->events[j]);
}

int _world_load_game_state(World *w, FILE *game_state_file)
{
    if (!w || !game_state_file) {
        HE("invalid arguments", "_world_load_game_state");
        return UINT_ERROR;
    }

    // read the number of turns
    char *buff = fgetll(game_state_file);
    if (!buff) {
        HE("could not read number of turns", "_world_load_game_state");
        return UINT_ERROR;
    }
    w->turn = atoi(buff);

    //read the level of the player
    buff = fgetll(game_state_file);
    if (!buff) {
        HE("could not read level of the player", "_world_load_game_state");
        return UINT_ERROR;
    }
    w->level = atoi(buff);

    //read the wallet of the player
    buff = fgetll(game_state_file);
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
        buff = fgetll(game_state_file);
        if (!buff) {
            HE("could not read quantity of a resource", "_world_load_game_state");
            return UINT_ERROR;
        }

        // add it to our wallet
        w->wallet[i] = atoi(buff); free(buff);
    }

    return !UINT_ERROR;
}

/*
 *TODO: Implement that function so that we can load a game
 */


int _world_load_map(World *w, FILE *game_state_file)
{
    if (!w || !game_state_file) {
        HE("invalid arguments", "_world_load_game_state");
        return UINT_ERROR;
    }

    //read the map size
    char *buff = fgetll(game_state_file);

    if(!buff) {
        // load the default map, if no map provided on this file

        int height  = config_get_int("map height");
        int columns = config_get_int("map columns");
        w->map_tiles = height * columns;
        w->map = map_new(w->tiles, w->num_tiles, map_gen_random, w->map_tiles);
        w->n_townhalls = 0;
        return !UINT_ERROR;

    }

    w->map_tiles = atoi(buff); free(buff);

    // read the number of town halls
    buff = fgetll(game_state_file);
    w->n_townhalls = atoi(buff); free(buff);

    Tile **tiles_map = oopsalloc(w->map_tiles, sizeof(Tile *), "_world_load_map");
    for(int i = 0; i < w->map_tiles; i++){

        //We read the id of the tile and create one canonic version of the tile with that id
        buff = fgetll(game_state_file);
        if(!buff) {
            HE("could not read part of the map", "_world_load_map");
            return UINT_ERROR;
        }

        int id = atoi(buff); free(buff);

        tiles_map[i] = _world_canonical_tile_copy(w, id);

        // read the tile's visibility
        buff = fgetll(game_state_file);
        int visible = atoi(buff); free(buff);
        tile_set_visible(tiles_map[i], visible);

        //Now we read the remaining resources of that tile and update it
        for(int k = 0; k < w->num_resources; k++){
            buff = fgetll(game_state_file);
            if(!buff) {
                HE("could not read part of the remaining resources of a tile", "_world_load_map");
            }
            int remain_resource = atoi(buff); free(buff);
            tiles_map[i] = tile_set_remaining_resources(tiles_map[i], k, remain_resource);
        }

        //if the tile have a building, we create it and associate it to that tile
        buff = fgetll(game_state_file);
        if(!buff) {
            HE("could not read a building", "_world_load_map");
            return UINT_ERROR;
        }
        id = atoi(buff); free(buff);
        if(id != -1){
            Building *b = _world_canonical_building_copy(w, id);
            if (!b) {
                HE("could not recognize building", "_world_load_map");
            }

            buff = fgetll(game_state_file);
            if(!buff) {
                HE("could not read a building", "_world_load_map");
                return UINT_ERROR;
            }
            int health = atoi(buff); free(buff);
            building_set_health(b, health);

            if(tile_build(tiles_map[i], b) == UINT_ERROR){
                printf("peta con tile index = %d, building id = %d\n", i, id);
                HE("could not connect tile and building", "_world_load_map");
            }
        }

        //if the tile have an event, we create it and associate it to that tile
        buff = fgetll(game_state_file);
        if(!buff) {
            HE("could not read an event", "_world_load_map");
            return UINT_ERROR;
        }
        id = atoi(buff); free(buff);
        if(id != -1){
            printf("peta con id = %d\n", id);
          Event *e = _world_canonical_event_copy(w, id);
          buff = fgetll(game_state_file);
          if(!buff) {
              HE("could not read an event", "_world_load_map");
              return UINT_ERROR;
          }
          int rem_turns = atoi(buff); free(buff);
          e = event_set_remaining_turns(e, rem_turns);

          if(tile_set_event(tiles_map[i], e) == NULL){
              HE("could not connect tile and event", "_world_load_map");
              return UINT_ERROR;
          }
      }
    }
    //At this point we have all the information of the file in tiles_map
    // We only need to create the real map with that information
    w->map = map_new(tiles_map, w->map_tiles, map_gen_standard, w->map_tiles);

    if (!w->map)
        return UINT_ERROR;

  return !UINT_ERROR;
}

World *world_new(char *archive) {

    World *w = NULL;
    w = oopsalloc(1, sizeof(World), "world_new");

    // random source
    w->rs = r_init(time(NULL));

    if (!archive)
        archive = config_get("general.initial_game_state");
    if (!archive) {
        HE("don't know where to load resources from", "world_new");
        free(w);
        return NULL;
    }

    FILE *game_file = fopen(archive, "r");
    if (!game_file) {
        HE("could not load game state", "world_new");
        free(w);
        return NULL;
    }

    //LOAD RESOURCES
    char *resources_db = fgetll(game_file);
    if (!resources_db) {
        HE("don't know where to load resources from, set up a 'asset_dbs.resources' entry in config", "world_new");
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
    char *tiles_db = fgetll(game_file);
    if (!tiles_db) {
        HE("don't know where to load tiles from, set up a 'asset_dbs.tiles' entry in config", "world_new");
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
    char *buildings_db = fgetll(game_file);
    if (!buildings_db) {
        HE("don't know where to load buildings from, set up a 'asset_dbs.buildings' entry in config", "world_new");
        resource_list_destroy(w->resources, w->num_resources);
        map_destroy(w->map); free(w);
        return NULL;
    }

    FILE *bf = fopen(buildings_db, "r");
    if (!bf) {
        HE("could not open buildings db file", "world_new");
        resource_list_destroy(w->resources, w->num_resources);
        map_destroy(w->map); free(w);
        return NULL;
    }

    w->buildings = load_buildings_from_file(bf, w->num_resources);
    if (!w->buildings) {
        HE("could not load tiles", "world_new");
        resource_list_destroy(w->resources, w->num_resources);
        map_destroy(w->map); free(w);
        return NULL;
    }

    fclose(bf);

    // load events
    char *events_db = fgetll(game_file);
    if (!events_db) {
        HE("don't know where to load events from, set up an 'asset_dbs.events' entry in config", "world_new");
        resource_list_destroy(w->resources, w->num_resources);
        map_destroy(w->map);
        building_list_destroy(w->buildings); free(w);
        return NULL;
    }

    FILE *ef = fopen(events_db, "r");
    if (!bf) {
        HE("could not open events db file", "world_new");
        resource_list_destroy(w->resources, w->num_resources);
        map_destroy(w->map);
        building_list_destroy(w->buildings); free(w);
        return NULL;
    }

    w->events = load_events_from_file(bf, w->num_resources);
    if (!w->events) {
        HE("could not load events", "world_new");
        resource_list_destroy(w->resources, w->num_resources);
        map_destroy(w->map);
        building_list_destroy(w->buildings); free(w);
        return NULL;
    }
    fclose(ef);

    // count tiles, buildings and events
    for (w->num_tiles = 0; w->tiles[w->num_tiles]; w->num_tiles++);
    for (w->num_buildings = 0; w->buildings[w->num_buildings]; w->num_buildings++);
    for (w->num_events = 0; w->events[w->num_events]; w->num_events++);

    // load initial game state
    if (UINT_ERROR == _world_load_game_state(w, game_file)) {
            HE("initial game state could not be loaded", "world_new");
        }

    // create the map
    _world_load_map(w, game_file);

    fclose(game_file);

    for(int i=0;i<N_SKILLS;i++){
      w->skills[i]=false;
    }

    //TODO: Change these lines below when issue #21 is fixed.
    int initial_tile = config_get_int("initial cursor");
    Tile *init_tile = world_tile_at_index(w, initial_tile);
    tile_set_visible(init_tile, true);

    return w;
}

int world_save_game(const World *w, char *filename)
{
    if (!w || !filename) {
        HE("invalid arguments", "world_save_game");
        return UINT_ERROR;
    }

    // open the file to save the game, overwritting if necessary
    FILE *f = fopen(filename, "w");
    if (!f) {
        HE("could not open file for saving", "world_save_game");
        return UINT_ERROR;
    }

    // save canonical entity sources
    fprintf(f, "%s\n", config_get("asset_dbs.resources"));
    fprintf(f, "%s\n", config_get("asset_dbs.tiles"));
    fprintf(f, "%s\n", config_get("asset_dbs.buildings"));
    fprintf(f, "%s\n", config_get("asset_dbs.events"));

    // save the current number of turns
    fprintf(f, "%d\n", w->turn);

    // save the current player level
    fprintf(f, "%d\n", w->level);

    // save the number of resources
    fprintf(f, "%d\n", w->num_resources);

    // save the wallet
    for (int i = 0; i < w->num_resources; i++)
        fprintf(f, "%d\n", w->wallet[i]);

    // save the number of tiles
    fprintf(f, "%d\n", w->map_tiles);

    // save the number of town halls
    fprintf(f, "%d\n", w->n_townhalls);

    // save the tiles
    for (int i = 0; i < w->map_tiles; i++) {
        Tile *t = world_tile_at_index(w, i);
        if (!t) {
            HE("could not read a tile, saved game is corrupt", "world_save_game");
            fclose(f);
            return UINT_ERROR;
        }

        // save the tile id
        fprintf(f, "%d\n", tile_get_id(t));

        // save the tile's visibility
        fprintf(f, "%d\n", tile_get_visible(t));

        // save the remaining resources of the tile
        for (int j = 0; j < w->num_resources; j++)
            fprintf(f, "%d\n", tile_get_remaining_resources(t, j));

        // if there's a bulding on this tile, save it
        Building *b = tile_get_building(t);
        if (b) {
            // save the building id
            fprintf(f, "%d\n", building_get_id(b));

            // save the buildings health
            fprintf(f, "%d\n", building_get_health(b));
        } else {
            fprintf(f, "-1\n");
        }

        // if there's an event on this tile, save it
        Event *e = tile_get_event(t);
        if (e) {
            // save the event's id
            fprintf(f, "%d\n", event_get_id(e));

            // save the remaining turns
            fprintf(f, "%d\n", event_get_num_turns(e));
        } else {
            fprintf(f, "-1\n");
        }

    }

    fclose(f);
    return !UINT_ERROR;
}

void world_destroy(World *w) {
  if(!w){
    HE("invalid parameters", "world_destroy");
    return;
  }

  building_list_destroy(w->buildings);
  map_destroy(w->map);
  tile_list_destroy(w->tiles);
  event_list_destroy(w->events);
  resource_list_destroy(w->resources, w->num_resources);
  free(w);
}

int world_start_game(World *w)
{
    if (!w) {
        HE("invalid arguments", "world_start_game");
        return UINT_ERROR;
    }

    w->last_turn_timestamp = time(NULL);
    return !UINT_ERROR;
}

int world_game_finished(World *w){
  if(!w){
    HE("invalid parameters", "world_game_finished")
    return UINT_ERROR;
  }

  if(w->wallet[3]<=0){
    int turn = world_get_turn(w);
    int score =0 ;
    for(int i=0;i<MAX_RESOURCES;i++){
      score+=w->wallet[i];
    }
    score+=100*turn;

    return score;
  }
  if(tile_get_building(world_tile_at_index(w,w->town_hall_index))){
    if(building_get_health(tile_get_building(world_tile_at_index(w,w->town_hall_index)))<=0){
      show_msg("townhall destroyed");
      int turn = world_get_turn(w);
      int score =0 ;
      for(int i=0;i<MAX_RESOURCES;i++){
        score+=w->wallet[i];
      }
      score+=100*turn;

      return score;
    }
  }

  return UINT_ERROR;
}

int _feed_rm(World* w, float coffee_consumed){
  w->wallet[2] -= coffee_consumed*w->wallet[6];
  if(w->wallet[2] < 0){
    w->wallet[6] += w->wallet[2];
    w->wallet[2] = 0;
  }
  return !UINT_ERROR;
}


//Maybe we need to call some other functions like create events
World *world_next_turn(World *w, int *tiles_to_update){
  if(!w){
    HE("invalid parameters", "world_next_turn");
    return NULL;
  }
  /*Feed marab in proportion 1 marab : MARAB_PROP coffee. If w->skills[EAT_SKILL_ID]=true, a skill is applied*/
  float proportion = MARAB_PROP;
  if(w->skills[EAT_SKILL_ID]){
    proportion *= EAT_SKILL_PROP;
  }
  _feed_rm(w, proportion);

  /*
  / Collect all the resources from the tiles with resources buildings and reduce by one
  / the turns of the active events
  */

  int num_tiles_to_update = 0;
  for(int i = 0; i < w->map_tiles; i++){
    int *resources = (int *) oopsalloc(MAX_RESOURCES, sizeof(int), "world_next_turn");
    if (TILE_NEXT_TURN_EVENT_ENDED == tile_next_turn(map_tile_at_index(w->map, i), resources))
        tiles_to_update[num_tiles_to_update++] = i;
    for(int j = 0; j < w->num_resources; j++){
      w->wallet[j] += resources[j];
    }
    free(resources);
  }

  /*
  / TODO: See how many events we want to manage with and modify this
  / so that the probabilties are consistent with that
  */

  int affecting_event = f_rnd(w->rs) * (w->num_events - 1);

  /*
  int i = 0;
  while(i < w->num_tiles){*/
    /* int tile_affected = aleat_num(0, w->num_tiles - 1); */
    /* int affecting_event = aleat_num(0, w->num_events - 1); */
  /*  int tile_affected = f_rnd(w->rs) * (w->num_tiles - 1);

    tile_set_event(w->tiles[tile_affected], w->events[affecting_event]);
    i += f_rnd(w->rs) * w->num_tiles;

    if(tile_get_building(w->tiles[tile_affected])){
      building_edit_health(tile_get_building(w->tiles[tile_affected]),event_get_damage(w->events[affecting_event]));
    }
  }
  */

  for(int i=0; i<w->map_tiles; i++){
    int probabilty= (int) i_rnd(w->rs)%100;

    if(probabilty<world_get_percentage_event(w)){
      Tile *t = map_tile_at_index(w->map, i);

      if(tile_get_event(t)){
        continue;
      }
      tile_set_event(t, w->events[affecting_event]);

      if(tile_get_building(t)){
        building_edit_health(tile_get_building(t), event_get_damage(w->events[affecting_event]));
        if(building_get_health(tile_get_building(t)) <= 0){
          tile_demolish_building(t);
        }
      }
    }
  }

w->turn++;

    int time_resource_id;
    if ((time_resource_id = config_get_int("general.time_resource")) != -1) {
        world_wallet_delta(w, time_resource_id, w->last_turn_timestamp - time(NULL));
    }
    w->last_turn_timestamp = time(NULL);
  return w;
}

int world_get_turn(World *w){
  if(!w){
    HE("invalid parameters", "world_get_turn")
    return INT_ERROR;
  }

  return w->turn;
}


int world_get_percentage_event(World *w){
  if(!w){
    HE("invalid parameters", "world_get_percentage_event")
    return INT_ERROR;
  }
  int turn = world_get_turn(w);
  int percentage = (int) 33-1000/(turn*log10(turn));

  return percentage;
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

Map *world_get_map(World *w)
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

  return w->map_tiles;
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

int world_get_num_townhalls(World *w)
{
  if (!w) {
    HE("invalid parameters", "world_get_num_townhalls");
    return UINT_ERROR;
  }

  return w->n_townhalls;
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
    for (int i = 0; i < w->map_tiles; tile_print(s, map_tile_at_index(w->map, i++)));

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

Tile *world_tile_at_index(const World *w, int tile_index)
{
    if (!w) {
        HE("invalid arguments", "world_tile_at_index");
        return NULL;
    }

    return map_tile_at_index(w->map, tile_index);
}

int world_get_player_level(World *w)
{
    if (!w)
        return UINT_ERROR;

    return w->level;
}

int world_build_on_tile(World *w, int tile_index, Building *b)
{
    if (!w || tile_index < 0 || tile_index >= w->map_tiles || !b) {
        HE("invalid arguments", "world_build_on_tile");
        return UINT_ERROR;
    }
    // if the tile is not visible, reject everything
    int visible = tile_get_visible(map_tile_at_index(w->map, tile_index));
    if(!visible)
        return WORLD_BUILD_NO_LIGHT;

    // if there are enemies on the tile, refuse to build
    if (tile_get_enemies(map_tile_at_index(w->map, tile_index)) != -1)
        return WORLD_BUILD_ENEMIES_PRESENT;

    // if tile is occupied, reject buildings that are not an upgrade
    int is_upgrade = building_is_upgrade(b, tile_get_building(map_tile_at_index(w->map, tile_index)));
    if (tile_get_building(map_tile_at_index(w->map, tile_index)) && !is_upgrade)
        return WORLD_BUILD_OCCUPIED;

    // if there is already a townhall constructed and we are trying to construct another one,
    // then it is rejected
    if(building_is_townhall(b) && w->n_townhalls == config_get_int("max_townhalls")) {
        return WORLD_BUILD_TOOMANY_TOWNHALLS;
    }

    // check if building can be build at the player's current level
    int unlock_level = building_get_unlocking_level(b);
    if (unlock_level > w->level)
        return WORLD_BUILD_NO_LEVEL;

    // check the player has enough resources
    for(int i = 0; i<MAX_RESOURCES; i++){
      int cost = building_get_cost(b, i);
      if (cost > w->wallet[i]){
        return WORLD_BUILD_NO_MONEY;
      }
    }

    // link building to tile
    if (UINT_ERROR == tile_build(map_tile_at_index(w->map, tile_index), b)) {
        HE("could not build for some reason", "world_build_on_tile");
        return UINT_ERROR;
    }

    // substract money from wallet
    for(int i = 0; i<MAX_RESOURCES; i++){
      int cost = building_get_cost(b, i);
      w->wallet[i] -= cost;
    }

    // if building is a town-hall, signal player level upgrade
    if (building_is_townhall(b)) {
        w->level++;
        w->n_townhalls++;
        w->town_hall_index=tile_index;
        return WORLD_BUILD_SUCCESS_LEVEL_UP;
    }

    // if this was a building upgrade, notify the controller
    else if (is_upgrade)
        return WORLD_BUILD_SUCCESS_UPGRADE;

    return WORLD_BUILD_SUCCESS;
}

int world_update_neighbours(World *w, int tile_index){
  if(!w || tile_index < 0){
    HE("Input error", "world_update_neighbours");
    return UINT_ERROR;
  }
  if(UINT_ERROR == map_update_neighbour_tiles(w->map, tile_index)){
    HE("Error updating map neighbours", "world_update_neighbours");
    return UINT_ERROR;
  }
  return !UINT_ERROR;
}

Building *_world_find_building_upgrade(World *w, int building_id){
  if(!w) return NULL;
  for (int i = 0; i < w->num_buildings; i++) {
    if(building_id + 1 == building_get_id(w->buildings[i])){
      return w->buildings[i];
    }
  }
  return NULL;
}

int world_upgrade_building(World *w, int tile_index){
  if(!w || tile_index < 0 || tile_index >= w->map_tiles) return -1;
  Map *m = world_get_map(w);
  Tile **list = map_get_map_tiles(m);
  Building *current = tile_get_building(list[tile_index]);
  if(!current) return WORLD_UPGRADE_NO_BUILDING;
  int id = building_get_id(current);
  //Finds upgrade building
  Building *b = _world_find_building_upgrade(w, id);
  if (!b) return WORLD_UPGRADE_MAX_LEVEL; //If no upgrade is encountered, it's in max level already

  //Builds it
  world_destroy_building_on_tile(w, tile_index);
  return world_build_on_tile(w, tile_index, b);
}

//TODO: Complete this

int world_get_price_exchange(int price, int resource_from, int resource_to, int building_level){
  //TODO: Load these from files
  if(resource_from == resource_to){
    return resource_from;
  }
  switch (resource_from) {
    case 0:
      if (resource_to == 1) return (int) (price * 1.5 * building_level);
      if (resource_to == 2) return (int) (price * 1.1 * building_level);
    case 5:
      if(resource_to == 4) return (int) (price * 0.1 * building_level);
    default: return price;
  }
}


World * world_set_skill(World *w, int skill, bool value){
  if(!w){
    HE("invalid parameters","world_set_skill")
    return NULL;
  }
  w->skills[skill]=value;

  return w;
}

bool world_get_skill(World *w, int skill){
  if(!w){
    HE("invalid parameters","world_get_skill")
    return false;
  }
  return w->skills[skill];
}

int world_repair_building(World *w, Building *b){
  if(!w || !b){
    HE("invalid parameters","world_repair_building")
    return -1;
  }
  int b_health=building_get_health(b);

  int b_id=building_get_id(b);
  int b_can_id;

  int i;
  for(i=0;i<w->num_buildings;i++){
    b_can_id=building_get_id(w->buildings[i]);
    if(b_can_id==b_id){
      break;
    }
  }
  int b_can_health=building_get_health(w->buildings[i]);

  building_set_health(b,b_can_health);

  return b_can_health-b_health;
}


int world_exchange(World *w, int tile_index, int price, int res_from, int res_to){
  if(!w) return INT_ERROR;
  Map *m = world_get_map(w);
  Tile **list = map_get_map_tiles(m);
  Building *current = tile_get_building(list[tile_index]);
  if(!building_is_market(current)){
    return WORLD_EXCHANGE_NOT_POSSIBLE;
  }
  int level = building_get_level(current);
  if(price > w->wallet[res_from]) return WORLD_EXCHANGE_NO_MONEY;
  int addition = world_get_price_exchange(price, res_from, res_to, level);
  w->wallet[res_from] -= price;
  w->wallet[res_to] += addition;
  return WORLD_EXCHANGE_DONE;
}

int world_get_skill_price(World *w, int skill_id){
  //TODO: Load these from files
  switch (skill_id) {
    case 0: return 20;
    case 1: return 40;
    case 2: return 80;
    case 3: return 1400;
    default: return 0;
  }
  return 0;
}

int world_hack(World *w, int tile_index, int price, int res_from, int res_to){
  if(!w) return INT_ERROR;
  Map *m = world_get_map(w);
  Tile **list = map_get_map_tiles(m);
  Building *current = tile_get_building(list[tile_index]);
  if(!building_is_coding_lab(current)){
    return WORLD_CODING_NOT_POSSIBLE;
  }
  int level = building_get_level(current);
  int cash;
  if(res_from == 4){
    cash = world_get_skill_price(w, res_to);
  }else{
    cash = price;
  }
  if(cash > w->wallet[res_from]) return WORLD_CODING_NO_ECTS;
  if(res_from == 5){
    int addition = world_get_price_exchange(price, res_from, res_to, level);
    w->wallet[res_to] += addition;
  }
  if(res_from == 4){
    world_set_skill(w, res_to, true);
  }
  w->wallet[res_from] -= cash;
  return WORLD_CODING_DONE;
}

void world_destroy_building_on_tile(World *w, int tile_index){
  Building *b = tile_get_building(world_tile_at_index(w, tile_index));
  if(!b) return;
  if(building_is_townhall(b)){
    w->n_townhalls--;
  }
  tile_demolish_building(world_tile_at_index(w, tile_index));
}
