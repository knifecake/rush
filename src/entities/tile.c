#include "tile.h"

#include <stdlib.h>

struct _Tile {
  int id;
  int building_id;
  int type;
  float resource_multipliers[MAX_RESOURCES];
  int remaining_resources[MAX_RESOURCES];
  bool visible;
  int enemies;
};

Tile *tile_new () {
  Tile *tile;
  tile = (Tile *)calloc(1, sizeof(Tile));
  if(!tile) {
    fprintf(stderr, "tile_new: cannot create tile.\n");
  }
  return tile;
}

void tile_destroy (Tile *tile) {
  if(!tile) return;
  free(tile);
}

int tile_get_id (Tile *tile) {
  if(!tile) {
    fprintf(stderr, "tile_get_id: invalid tile.\n");
    return -1;
  }
  return tile->id;
}

int tile_get_building_id (Tile *tile) {
  if(!tile) {
    fprintf(stderr, "tile_get_building_id: invalid tile.\n");
    return -1;
  }
  return tile->building_id;
}

float tile_get_resource_multipliers (Tile *tile, int multiplier_id) {
  if(!tile) {
    fprintf(stderr, "tile_get_resource_multipliers: invalid tile.\n");
    return -1;
  }
return tile->resource_multipliers[multiplier_id];
}

int tile_get_remaining_resources (Tile *tile, int resource_id) {
  if(!tile) {
    fprintf(stderr, "tile_get_remaining_resources: invalid tile.\n");
    return -1;
  }
  return tile->remaining_resources[resource_id];
}

bool tile_get_visible (Tile *tile) {
  if(!tile) {
    fprintf(stderr, "tile_get_visible: invalid tile.\n");
    return false;
  }
  return tile->visible;
}

int tile_get_enemies (Tile *tile) {
  if(!tile) {
    fprintf(stderr, "tile_get_enemies: invalid tile.\n");
    return -1;
  }
  return tile->enemies;
}

Tile *tile_set_id (Tile *tile, int id) {
  if(!tile) {
    fprintf(stderr, "tile_set_id: invalid tile.\n");
    return NULL;
  }
  if(id<0) {
    fprintf(stderr, "tile_set_id: invalid id.\n");
    return NULL;
  }
  tile->id=id;

  return tile;
}
