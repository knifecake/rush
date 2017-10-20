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

World *world_new(Tile **, Building **);

void world_destroy(World *);

World *world_load_map(World *);

World *world_player_turn(World *, Player *);

World *world_ai_turn();


#endif
