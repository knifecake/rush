/*
 * building.h
 *
 * A building which can be placed on a conquered tile.
 *
 *  - building type -- ID:
 *  - unlocks_at: the minimum level the player must have to be able to build this
 *  - cost: resources needed to place this building on a tile
 *  - health :=)
 *  - level :)
 *  - base_resources: quantity of resources returned each time
 *  - tile_id: where is this building?
 *
 *
 * Actions:
 *  - level_up
 *  - collect_resources
 *  - delta_health: increases health by specified amount
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct _building Building;

Building *building_new(size_t id);

void building_destroy(Building *);

Building *building_level_up(Building *);

Building *building_place(Building *, int);

Building *building_edit_health(Building *, int);

/*GET FUNCTIONS*/

size_t building_get_id(Building *);

int building_get_level(Building *);

int building_get_unlocking_level(Building *);

int building_get_health(Building *);

int building_get_cost(Building *);

int building_get_tile_id(Building *);

int building_get_base_resources(Building *)
