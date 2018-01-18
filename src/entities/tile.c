/*
 * PPROG Game – Morzilla Firefox 2018
 *
 * Authors: Miguel Baquedano, Sergio Cordero, Elias Hernandis
 *          and Rafael Sánchez.
 *
 * Lead author: <replace me>
 */


#include "tile.h"

#include "../lib/error_handling.h"

#include <stdlib.h>
#include <string.h>

struct _Tile {
  int id;
  char sprite[MAX_SPRITE_NAME + 1];
  float resource_multipliers[MAX_RESOURCES];
  int remaining_resources[MAX_RESOURCES];
  int enemies;
  Building *building;
  bool visible;
  Event* event;
};

Tile *tile_new (int id, const char *sprite, float *resource_multipliers, int *remaining_resources, int enemies) {
  if(!sprite){
    HE("cannot create tile, missing sprite name", "tile_new")
    return NULL;
  }

  if (strlen(sprite) > MAX_SPRITE_NAME){
    HE("cannot create tile, sprite name too long", "tile_new")
    return NULL;
  }

  if(!resource_multipliers){
    HE("cannot create tile, missing resource multiplier", "tile_new")
    return NULL;
  }

  if(!remaining_resources){
    HE("cannot create tile, missing resource multiplier", "tile_new")
    return NULL;
  }

  Tile *tile = (Tile *)calloc(1, sizeof(Tile));

  if(!tile) {
    HE("cannot create tile, out of memory", "tile_new")
    return NULL;
  }

  tile->id = id;
  strcpy(tile->sprite, sprite);

  for (size_t i = 0; i<MAX_RESOURCES; i++)
    tile->resource_multipliers[i] = resource_multipliers[i];

  for (size_t i = 0; i<MAX_RESOURCES; i++)
    tile->remaining_resources[i] = remaining_resources[i];

  tile->enemies = enemies;
  tile->building = NULL;
  tile->visible = false;
  tile->event = NULL;

  return tile;
}

void tile_destroy (Tile *tile) {
  if(!tile) return;
  if (tile->building) {
      building_destroy(tile->building);
  }
  if(tile->event){
    event_destroy(tile->event);
    tile->event=NULL;
  }
  free(tile);
}

int tile_get_id (Tile *tile) {
  if(!tile) {
    HE("invalid tile", "tile_get_id")
    return INT_ERROR;
  }
  return tile->id;
}

Building *tile_get_building (Tile *tile) {
  if(!tile) {
    HE("invalid tile", "tile_get_building")
    return NULL;
  }
  return tile->building;
}

float tile_get_resource_multipliers (Tile *tile, int resource_id) {
  if(!tile) {
    HE("invalid tile", "tile_get_resource_multipliers")
    return INT_ERROR;
  }
return tile->resource_multipliers[resource_id];
}

int tile_get_remaining_resources (Tile *tile, int resource_id) {
  if(!tile||resource_id < 0) {
    HE("invalid tile", "tile_get_remaining_resources")
    return INT_ERROR;
  }
  return tile->remaining_resources[resource_id];
}

bool tile_get_visible (Tile *tile) {
  if(!tile) {
    HE("invalid tile", "tile_get_visible")
    return false;
  }
  return tile->visible;
}

int tile_get_enemies (Tile *tile) {
  if(!tile) {
    HE("invalid tile", "tile_get_enemies")
    return INT_ERROR;
  }
  return tile->enemies;
}

Event *tile_get_event (Tile *tile){
  if(!tile) {
    HE("invalid tile", "tile_get_event")
    return NULL;
  }
  return tile->event;
}

char *tile_get_sprite(Tile *t)
{
    if (!t) {
        HE("invalid arguments", "tile_get_sprite");
        return NULL;
    }

    return t->sprite;
}

Tile *tile_set_event (Tile *tile, Event *event){
  if(!tile){
    HE("invalid tile", "tile_set_event")
    return NULL;
  }
  if(!event){
    HE("invalid event", "tile_set_event")
    return NULL;
  }
  if(tile->event){
    event_destroy(tile->event);
    tile->event = NULL;
  }
  tile->event = event_copy (event);
  if(!tile->event){
    HE("couldn't copy the event", "tile_set_event")
    return NULL;
  }
  return tile;
}

int tile_conquer(Tile *tile)
{
    if (!tile) {
        HE("invalid arguments", "tile_conquer");
        return UINT_ERROR;
    }

    tile->enemies = -1;
    return !UINT_ERROR;
}

int tile_set_visible(Tile *t, bool visible){
  if(!t){
    HE("Input error", "tile_set_visible")
    return UINT_ERROR;
  }
  t->visible = visible;
  return !UINT_ERROR;
}

int tile_build (Tile *tile, Building *bp){
  if (!tile){
    HE("invalid tile", "tile_build")
    return UINT_ERROR;
  }
  if (!bp){
    HE("invalid building", "tile_build")
    return UINT_ERROR;
  }
  tile->building = building_copy(bp);
  if (!tile->building) {
      HE("could not duplicate building", "tile_build");
      return UINT_ERROR;
  }

  return !UINT_ERROR;
}

int tile_demolish_building(Tile *tile){
  if(!tile){
    HE("invalid tile", "tile_demolish_building")
    return UINT_ERROR;
  }
  if(!tile->building) return !UINT_ERROR;
  building_destroy(tile->building);
  tile->building = NULL;
  return !UINT_ERROR;
}

/* In order to calculate the number of resources each tile returns, follow this order:
        1. First of all, it checks if the remining resource is enough to collect the
        base resources of its building. If not, it catch all that remains.

        2. After that, it multiplies that value by the tile multiplier and converts that
        value to an int

        3. After that, it checks if an event is active in that tile. If that is the case,
        it multiplies the previous value by the event multiplier and converts the result to an int again */

/*int tile_collect_resources(Tile * tile, int resource_id){
  if (!tile){
    HE("invalid tile pointer", "tile_collect_resources")
    return INT_ERROR;
  }
  if (resource_id < 0 || resource_id > MAX_RESOURCES){
    HE("invalid resource_id", "tile_collect_resources")
    return INT_ERROR;
  }
  if(!tile->building){
    HE("building is null","tile_collect_resources")
    return 0;
  }
  int base_resource = building_get_base_resources(tile->building, resource_id);
  if (tile->remaining_resources [resource_id]< base_resource){
    base_resource = tile->remaining_resources[resource_id];
    tile->remaining_resources[resource_id] = 0;
  }else{
    tile -> remaining_resources[resource_id] -= base_resource;
  }
  return (int) base_resource * tile->resource_multipliers[resource_id];
}*/

int tile_collect_resources(Tile *tile, int resource_index){
  int base;
  float multiplier;
  if(resource_index == -1){
    return 0;
  }
  if(tile_get_building(tile)){
    base = building_get_base_resources(tile_get_building(tile), resource_index);

    int rr = tile_get_remaining_resources(tile, resource_index);
    rr -= base;
    if(rr < 0){
      base += rr;
      rr = 0;
    }
    tile->remaining_resources[resource_index] = rr;
    if (tile->event){
      multiplier = event_get_mult(tile->event)[resource_index];
      base = (int)base*multiplier;
    }
    return base * tile_get_resource_multipliers(tile,resource_index);
  }
  return 0;
}


int tile_next_turn(Tile *tile, int *resources){
  if (!tile){
    HE("invalid tile pointer", "tile_next_turn")
        return UINT_ERROR;
  }
  if (!resources){
    HE("invalid resources pointer", "tile_next_turn")
        return UINT_ERROR;
  }
  for(int i = 0; i < MAX_RESOURCES; ++i){
    resources[i] = tile_collect_resources(tile, i);
  }
  if(tile->event){
    if(!event_next_turn(tile->event)){
      HE("error nexting event turn", "tile_next_turn")
          return UINT_ERROR;
    }
    if(event_get_num_turns(tile->event) == 0){
      event_destroy(tile->event);
      tile->event = NULL;
      return TILE_NEXT_TURN_EVENT_ENDED;
    }
  }
  return TILE_NEXT_TURN_OK;
}
int tile_get_resource_per_turn(Tile *tile, int resource_index){
  int base;
  float multiplier;
  if(resource_index == -1){
    return 0;
  }
  if(tile_get_building(tile)){
    base = building_get_base_resources(tile_get_building(tile), resource_index) * tile_get_resource_multipliers(tile,resource_index);
    if (tile->event){
      multiplier = event_get_mult(tile->event)[resource_index];
      base = (int)base*multiplier;
    }
    return base;
  }
  return 0;
}

int tile_find_resource_index(Tile* t){
  for(int i = 0; i < MAX_RESOURCES; ++i){
    if (tile_get_resource_per_turn(t, i) != 0){
      return i;
    }
  }
  return -1;
}

Tile *tile_copy(Tile* src){
  if(!src){
    HE("Input error", "tile_copy")
    return NULL;
  }
  Tile *dest = tile_new(src->id,src->sprite, src->resource_multipliers, src->remaining_resources, src->enemies);
  if(!dest){
    HE("Error copying the tile", "tile_copy")
    return NULL;
  }

  Building *b;
  Event *e;
  if ((b = tile_get_building(src)))
    tile_build(dest, b);

  if ((e = tile_get_event(src)))
    tile_set_event(dest, e);

  return dest;
}

void tile_print(FILE *f, Tile *t) {
    if (!f || !t) {
        HE("invalid arguments", "tile_print")
        return;
    }

    fprintf(f, "Tile %d (%s):", t->id, t->sprite);

    fprintf(f, "\n - resource no.:         ");
    for (int i = 0; i < MAX_RESOURCES; fprintf(f, "%8d ", i++));

    fprintf(f, "\n - remaining resources:  ");
    for (int i = 0; i < MAX_RESOURCES; fprintf(f, "%8d ", t->remaining_resources[i++]));

    fprintf(f, "\n - resource multipliers: ");
    for (int i = 0; i < MAX_RESOURCES; fprintf(f, "%8g ", t->resource_multipliers[i++]));

    fprintf(f, "\n - enemies: %d\n", t->enemies);
}
