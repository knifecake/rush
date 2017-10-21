/*
 * Building loader.
 *
 * Contains functions to read a list of buildings from a file.
 */


#include "../entities/building.h"

/*
 *  Returns a null-terminated list of pointers to buildings with buildings loaded
 *  from a file. Thus, the length of the list may be detemined by looking for the
 *  first NULL pointer of the list. Never loads more than MAX_BUILDINGS buildings.
 *  It's arguments are:
 *   -file to read buildings from
 *   -
 *
 * The building database file must conform to this format:
 *
 *      2           # number of buildings in file
 *
 *      1           # (int) building_id
 *      4           # (int) current level of the building
 *      2           # (int) player's level required to build it
 *
 *      1800        # (int) building's health points
 *      1550        # (int) buiding's leveling cost
 *
 *      1532        # (int) quantity of resource 1 returned each time
 *      29032       # (int) quantity of resource 2 returned each time
 *      ...
 *      45078       # (int) quantity of resource n returned each time
 *
 * See the example building database file provided in assets/building.txt for
 * additional details on the file format.
 */
Building **load_buildings_from_file(FILE *, int);

 /*
  * Frees a list of pointers to buildings. Assumes list is len buildings long.
  *
  * TODO: consider this a utility function. Maybe it should be somewhere else.
  */
void building_list_destroy(Building **);
