#ifndef __BUILDING_H__
#define __BUILDING_H__
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

#include "resource.h"

typedef struct _Building Building;


/*
 * This function saves memory for a new bulding, recieve the id of the building
 * as an argument, then sets every other value according to the established data
 * If it fails, NULL is returned.
 */
Building *building_new(int, int, int, int, int, int *, const char *);

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
 * Adds or subtract depending on the increment. If the value were going to be
 * negative, instead it is 0.
 * Returns a NULL value if an error occurs.
 */
Building *building_edit_health(Building *, int);

/*
 * GETTERS. Returns -1 on error.
 */

int building_get_id(Building *);

char *building_get_name(Building *);

int building_get_level(Building *);

int building_get_unlocking_level(Building *);

int building_get_health(Building *);

int building_get_cost(Building *);

char *building_get_sprite(Building *);
/*
 * Returns the base resource of a building given the id of the resource.
 * If it fails, returns -1.
 */
int building_get_base_resources(Building *, const int);

void building_print(FILE *s, Building *bp);
#endif
