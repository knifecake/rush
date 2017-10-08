/*
 * Tile loader.
 *
 * Contains functions to read a list of tiles from a file.
 */


#include "../entities/tile.h"

/*
 * Returns a null-terminated list of pointers to tiles. Thus, the length of the
 * list may be determined by looking for the first NULL pointer on the list.
 * Never loads more than MAX_TILES tiles. It's arguments are:
 *  - file to read tiles from
 *  - number of resources to look for on the resources list of each tile. Get this number from the top of the resources database file.
 *
 * The tile database file must conform to this format:
 *
 *      1         # (int) number of tiles in file
 *
 *      1         # (int) tile_id
 *      forest    # (char *) sprite
 *
 *      10.5      # (float) resource 1 multiplier
 *      12.0      # (float) resource 2 multiplier
 *      ...
 *      0.89      # (float) resource n multiplier
 *
 *      177261    # (int) quantity of resource 1 available
 *      12001     # (int) quantity of resource 2 available
 *      ...
 *      0         # (int) quantity of resource n avaiable
 *
 *      120       # number of enemies
 *
 * See the example tile database file provided in assets/tiles.txt for
 * additional details on the file format.
 */
Tile **load_tiles_from_file(FILE *, int);

/*
 * Frees a list of pointers to tiles. Assumes list is len tiles long.
 *
 * TODO: consider this a utility function. Maybe it should be somewhere else.
 */
void tile_list_destroy(Tile **);
