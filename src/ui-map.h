#ifndef __UI_MAP__
#define __UI_MAP__

#include "entities/world.h"

/*
 * UIMap
 *
 * A Map view displayed on the center of the screen.
 */
typedef struct _UIMap UIMap;

/*
 * Creates a new map component bound the the given world. Maybe additional
 * getters are needed on World to be able to access the internal world map
 * structure.
 */
UIMap *ui_map_new(World *w);

/*
 * A list of possible directions to move to.
 */
typedef enum { UP, LEFT, DOWN, RIGHT, HERE } UIMapVector;

/*
 * Moves the cursor if it's possible to do so.
 */
int ui_map_move_cursor(UIMap *m, UIMapVector dir);

/*
 * Deletes the previous cursor and draws the new one.
 */
void ui_map_update_cursor(UIMap *, int cursor);

/*
 * Returns the focused tile's index or UINT_ERROR on error.
 */
int ui_map_get_cursor(UIMap *m);

/*
 * Redraws a spectific Tile (if in-screen) on the map. This function gets called,
 * for instance, when a building is constructed in one of the tiles.
 */
void ui_map_redraw_tile(UIMap *m, int tile_index);

/*
 * Redraws all neighbours of a specific tile in the map.
 */
void ui_map_redraw_neighbours(UIMap *, int);

/*
* Draws a new map starting from (x,y).
*
* Before drawing, you should update the UIMap yourself.
*/
void ui_map_draw(UIMap *m);

/*
 * Prints all cursors in visible screen layer by layer
 */
void ui_map_layers_draw(UIMap *m);
/*
 * Frees all memory allocated by ui_map_new.
 */
void ui_map_destroy(UIMap *m);

#endif
