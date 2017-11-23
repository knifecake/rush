#ifndef __WORLD_H__
#define __WORLD_H__

#include "tile.h"
#include "building.h"

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
 * Constructs the internal map for the world, chosing from the available tiles.
 */
World *world_load_map(World *);

/*
 * Lets player perform actions.
 */
World *world_player_turn(World *);

/*
 * Updates the world between turns of the player.
 */
World *world_ai_turn(World *);

/*
 * Moves the cursor by one tile in the specified direction.
 *
 * See `lib/game_controller.h` for details on possible directions.
 */
World *world_move_cursor(World *w, int dir);

/*
 * Returns a pointer to the tile under the cursor, NULL on error.
 */
Tile *world_get_current_tile(World *w);

/*
 * Returns a pointer to the list of available buildings.
 */
Building **world_get_buildings(World *w);

/*
 * Returns the number of buildings that world_get_buildings returns, UINT_ERROR
 * in case of failure.
 */
int world_get_num_buildings(World *w);

/*
 * Prints what's inside the world for debugging.
 */
void world_print(FILE *s, World *w);

#endif
