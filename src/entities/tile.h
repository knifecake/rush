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
 *
 */
#ifndef __TILE_H__
#define __TILE_H__

#include "building.h"

#include "../error_handling.h"

#include <stdbool.h>

#include <string.h>

#define MAX_SPRITE_NAME 32

/*
 * Maximum number of tiles the program supports. Unlike MAX_RESOURCES, setting
 * a large MAX_TILES does not incur in a space penalty, as tiles are stored
 * dynamically.
 */
#define MAX_TILES 100

typedef struct _Tile Tile;

/*
 * Creates a new tile given the id, sprite name, resource_multipliers,
 * remaining_resources and enemies.
 * Returns NULL on error.
 */
Tile *tile_new (int, const char *, float *, int *, int);

/*
 * Frees memory allocated by tile.
 */
void tile_destroy (Tile *);

/*
 * GETTERS. Returns NULL, -1 or false on error.
 */
int tile_get_id (Tile *);

int tile_get_building_type (Tile *);

float tile_get_resource_multipliers (Tile *, int);

int tile_get_remaining_resources (Tile *, int);

bool tile_get_visible (Tile *);

int tile_get_enemies (Tile *);

/*Tile *tile_set_id (Tile *, int); Is it useful?*/

/*
 * Given a tile and a building, links them.
 * Returns NULL on error.
 */
Tile *tile_build (Tile *, Building *);

/*
 * Given a tile and a resource id, return amount of resources.
 * Returns -1 on error.
 */
int tile_collect_resources(Tile *, int);

/*
 * Prints a Tile, for DEBUG purposes.
 */
void tile_print(FILE *, Tile *);

 #endif
