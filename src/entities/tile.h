/*
 * PPROG Game – Morzilla Firefox 2018
 *
 * Authors: Miguel Baquedano, Sergio Cordero, Elias Hernandis
 *          and Rafael Sánchez.
 *
 * Lead author: <replace me>
 */


#ifndef __TILE_H__
#define __TILE_H__
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

#include "building.h"
#include "event.h"

#include <stdbool.h>

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
 * GETTERS. Returns NULL, INT_ERROR or false on error.
 */
int tile_get_id (Tile *);

Building *tile_get_building (Tile *);

float tile_get_resource_multipliers (Tile *, int);

int tile_get_remaining_resources (Tile *, int);

bool tile_get_visible (Tile *);

int tile_get_enemies (Tile *);

Event *tile_get_event (Tile *tile);

char *tile_get_sprite(Tile *t);

/*Tile *tile_set_id (Tile *, int); Is it useful?*/

/*
 * Creates a copy of an event loaded and insert it into tile.
 * It ALLOCATES memory.
 * Returns NULL on error. Return modified tile on success.
 */
Tile *tile_set_event (Tile *, Event *);

/*
 * Sets visibility of a tile, true for visible, false for not visible.
 * Returns UINT_ERROR on error.
 */
int tile_set_visible(Tile *, bool);

/*
 * Marks the tile as conquered (sets enemies to -1).
 */
int tile_conquer(Tile *tile);

/*
 * TODO: Decide what tile_next_turn has to manage.
 * - event: calls function event_next_turn and then checks if the event has
 *          finished. If so, it frees the mem allocated by the event and sets
 *          the event pointer to NULL again.
 *
 *
 * Returns
 *  - TILE_NEXT_TURN_EVENT_ENDED if an event just ended.
 *  - TILE_NEXT_TURN_OK
 */
#define TILE_NEXT_TURN_EVENT_ENDED 2
#define TILE_NEXT_TURN_OK 1
int tile_next_turn (Tile *, int *);
/*
 * Given a tile and a building, links them, does not allocate memory.
 * Returns UINT_ERROR on error. TODO: Should this allocate memory itself?
 */
int tile_build (Tile *, Building *);

/*
 * Given a tile and a resource id, return amount of resources.
 * Returns -1 on error.
 */
int tile_collect_resources(Tile *, int);
/*
 * Given a resource id and a tile, return the amount of resources multiplied by
 * the event if it exists. Returns 0 if there's no building.
 */
int tile_get_resource_per_turn(Tile *, int);

/*
 * Finds the index of the only resource that will be collected
 */
int tile_find_resource_index(Tile *);
/*
 * Returns a copy of the given tile or NULL on error.
 */
 Tile *tile_copy(Tile*);
/*
 * Prints a Tile, for DEBUG purposes.
 */
void tile_print(FILE *, Tile *);

 #endif
