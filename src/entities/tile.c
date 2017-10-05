#include "tile.h"

#include <stdlib.h>
#include <stdbool.h>

struct _Tile {
  int tile_id;
  int building_type;
  float resource_multipliers[MAX_RESOURCES];
  int remaining_resources[MAX_RESOURCES];
  bool visible;
  int enemies;
  char *sprite;
} _Tile;
