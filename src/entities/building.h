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


/*
 * This function saves memory for a new bulding, recieve the id of the building
 * as an argument, then sets every other value according to the established data
 * If it fails, NULL is returned.
 */
Building *building_new(int id);

/*
 * Frees memory allocated by building_new, it will only free it if the pointer
 * is not NULL.
 */
void building_destroy(Building *);

/*
 * Increases level of the building, as well as the other parameters of it.
 * Returns a NULL value if an error occurs.
 */
Building *building_level_up(Building *);

/*
 * Associates a building with the id of a tile.
 * Returns a NULL value if an error occurs.
 */
Building *building_place(Building *, int);

/*
 * Adds or subtract depending on the increment. If the value were going to be
 * negative, instead it is 0.
 * Returns a NULL value if an error occurs.
 */
Building *building_edit_health(Building *, int);

/*GETTERS*/

int building_get_id(Building *);

int building_get_level(Building *);

int building_get_unlocking_level(Building *);

int building_get_health(Building *);

int building_get_cost(Building *);

int building_get_tile_id(Building *);

int building_get_base_resources(Building *);
