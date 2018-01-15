#ifndef __UI_BUILD_PANEL__
#define __UI_BUILD_PANEL__

#include "entities/world.h"

/*
 * UIBuildPanel
 *
 * A Panel to select buildings.
 */
typedef struct _UIBuildPanel UIBuildPanel;

/*
 * Creates a new panel to display different buildings to be built.
 */
UIBuildPanel *ui_build_panel_new();

/*
 * A list of possible directions to move to.
 */
typedef enum { UP_BDIR = -2, LEFT_BDIR = -3, DOWN_BDIR = -1, RIGHT_BDIR = -4, HERE_BDIR = -5 } UIBuildDirection;

/*
 * Moves the cursor if it's possible to do so.
 */
int ui_build_panel_move_cursor(UIBuildPanel *bp, UIBuildDirection dir);

/*
 * Deletes the previous cursor and draws the new one.
 */
void ui_build_panel_update_cursor(UIBuildPanel *bp, int cursor);

/*
 * Returns the focused tile's index or UINT_ERROR on error.
 */
int ui_build_panel_get_cursor(UIBuildPanel *bp);

/*
 * Draws the build panel from (x,y).
 *
 */
void ui_build_panel_draw(UIBuildPanel *bp, int x, int y);

/*
 * Controls the build panel
 */
Building *ui_build_panel_control(UIBuildPanel *buildpanel);

/*
 * Frees all memory allocated by ui_map_new.
 */
void ui_build_panel_destroy(UIBuildPanel *bp);

#endif
