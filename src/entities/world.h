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
World *world_new(Tile **, Building **);

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
World *world_player_turn(World *, Player *);

/*
 * Updates the world between turns of the player.
 */
World *world_ai_turn();


#endif
