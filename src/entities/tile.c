#include "tile.h"

#include "../error_handling.h"

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

  return tile;
}

void tile_destroy (Tile *tile) {
  if(!tile) return;
  free(tile);
}

int tile_get_id (Tile *tile) {
  if(!tile) {
    HE("invalid tile", "tile_get_id")
    return -1;
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
    return -1;
  }
return tile->resource_multipliers[resource_id];
}

int tile_get_remaining_resources (Tile *tile, int resource_id) {
  if(!tile) {
    HE("invalid tile", "tile_get_remaining_resources")
    return -1;
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
    return -1;
  }
  return tile->enemies;
}

/*Tile *tile_set_id (Tile *tile, int id) {
  if(!tile) {
    HE("tile_set_id: invalid tile.\n");
    return NULL;
  }
  if(id<0) {
    HE("tile_set_id: invalid id.\n");
    return NULL;
  }
  tile->id=id;

  return tile;
}*/

Tile *tile_build (Tile *tile, Building *bp){
  if (!tile){
    HE("invalid tile", "tile_build")
    return NULL;
  }
  if (!bp){
    HE("invalid building", "tile_build")
    return NULL;
  }
  tile->building = bp;
  return tile;
}

int tile_collect_resources(Tile * tile, int resource_id){
  if (!tile){
    HE("invalid tile pointer", "tile_collect_resources")
    return -1;
  }
  if (resource_id < 0 || resource_id > MAX_RESOURCES){
    HE("invalid resource_id", "tile_collect_resources")
    return -1;
  }
  int base_resource = building_get_base_resources(tile->building, resource_id);
  if (tile->remaining_resources [resource_id]< base_resource){
    base_resource = tile->remaining_resources[resource_id];
    tile->remaining_resources[resource_id] = 0;
  }else{
    tile -> remaining_resources[resource_id] -= base_resource;
  }
  return (int) base_resource * tile->resource_multipliers[resource_id];
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
