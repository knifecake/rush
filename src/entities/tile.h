/*
 * tile.h
 *
 * A place is the basic unit of space. It has the following attributes:
 *  - tile_id: unique identifier for this cell
 *  - building_type: id of building type, -1 if no building
 *  - resource_multipliers: the amounts of each resource that can be obtained from this tile
 *  - remaining_resources: the amount of each resource remaining on this tile
 *  - visible: whether this tile is visible to the player
 *  - enemies: the number of enemies currently on this tile, -1 if conquered
 *  - sprite: tile image
 *
 * Actions:
 *  - build
 *  - draw_tile(x, y) draws the tile starting from (x, y)
 *
 */

 /*
  *This constant is used in the tiles to limit the amount of materials you can obtain
  *from this tile.
  *
  */
#define MAX_RESOURCES 10

Tile *tile_new ();

void tile_destroy (Tile *);

int tile_get_id (Tile *);

int tile_get_building_type (Tile *);

float tile_get_resource_multipliers (Tile *);

int tile_get_remaining_resources (Tile *);

bool tile_get_visible (Tile *);

int tile_get_enemies (Tile *);

Tile *tile_set_id (Tile *, int);

Tile *tile_set_building_type (Tile *, int);

Tile *tile_build (Tile *, Building *);
