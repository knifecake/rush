#ifndef __WORLD_H__
#define __WORLD_H__

#include "tile.h"
#include "building.h"
#include "map.h"

#define COST_RESOURCE 0


/*
 * world.h
 *
 * Properties:
 *  - population
 *  - resources[]
 *  - tiles[][]
 *  - townhall_tile
 *  - selected_tile
 *
 *  - attack_probability
 *  - day/time/turn
 *  -
 *
 * Actions:
 */

typedef struct _World World;

/*
 * Returns a new world object. Receives an array of pointers to all the
 * available tiles and buildings. Returns NULL on failure.
 */
World *world_new(void);

/*
 * Destroys a world, freeing all associated objects.
 *
 * Does nothing on invalid input.
 */
void world_destroy(World *);

/*
 * Updates the world between turns of the player.
 */
World *world_next_turn(World *);


/*
 * Returns the wallet of the player
 */

int *world_get_wallet(World *);

/*
 * Returns the number of resources in World or
 * UINT_ERROR on error.
 */
int world_get_num_resources(World *w);

/*
 * Returns a list of pointers to the resources
 * stored in World, null on error.
 */
Resource **world_get_resources(World *w);

/*
 * Returns the quantity of the resource with
 * resource_id that the player has collected,
 * UINT_ERROR on error.
 */
int world_get_resource_quantity(World *w, int resource_id);

/*
 * Returns a pointer to the list of available buildings.
 */
Building **world_get_buildings(World *w);

/*
 * Returns a pointer to the list of available tiles.
 */
Tile **world_get_tiles(World *w);
/*
 * Returns the number of buildings that world_get_buildings returns, UINT_ERROR
 * in case of failure.
 */
int world_get_num_buildings(World *w);

/*
 * Returns a pointer to the list of available tiles.
 */
Tile **world_get_tiles(World *w);

/*
 * Returns the number of tiles that world_get_tiles returns, UINT_ERROR
 * in case of failure.
 */
int world_get_num_tiles(World *w);

/*
 * Returns the number of tiles the map has. UINT_ERROR on error
 */
Map *world_get_map(World *w);

/*
 * Returns a pointer to the list of available events.
 */
Event **world_get_events(World *w);

/*
 * Returns the number of tiles that world_get_tiles returns, UINT_ERROR
 * in case of failure.
 */
int world_get_num_events(World *w);

int world_get_player_level(World *w);


/*
 * Prints what's inside the world for debugging.
 */
void world_print(FILE *f, World *w);

/*
 * Modifies the quantity of a resource held in the wallet by delta (can be
 * positive or negative). Returns UINT_ERROR on error.
 */
int world_wallet_delta(World *w, int resource_id, int delta);

/*
 * Returns the tile at the given index, NULL on error.
 */
Tile *world_tile_at_index(World *w, int tile_index);

/*
 * Builds on the current tile if the following are met:
 *  - Player has building unlocking level
 *  - Player has money
 *
 *  returns UINT_ERROR if an error ocurred. In other circumstances it returns
 *  one of the following:
 */

/*
 * Player does not have enough resources to build this.
 */
#define WORLD_BUILD_NO_MONEY 2

/*
 * Player's level is not hight enough. This building hasn't unlocked.
 */
#define WORLD_BUILD_NO_LEVEL 3

/*
 * Building was successfully constructed.
 */
#define WORLD_BUILD_SUCCESS 4

/*
 * Building was successfully construted and caused the player to move 1 level
 * up.
 */
#define WORLD_BUILD_SUCCESS_LEVEL_UP 5

/*
 * Building could not be constructed because there is a building of a different
 * kind on this tile.
 */
#define WORLD_BUILD_OCCUPIED 6

/*
 * Building was successfully constructed and overrides the previous building
 * upgrading it.
 */
#define WORLD_BUILD_SUCCESS_UPGRADE 7

/*
 * Building wasn't successfully constructed due to tile was not visible
 */
#define WORLD_BUILD_NO_LIGHT 8

int world_build_on_tile(World *w, int tile_index, Building *b);

#endif
