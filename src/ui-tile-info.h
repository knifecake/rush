#ifndef __UI_TILE_INFO__
#define __UI_TILE_INFO__

#include "entities/world.h"
#include "ui.h"

/*
 * UITileInfo panel
 *
 * A tile info panel intended to be displayed on the right of the sidebar.
 *
 * Notice: this component's new/destroy functions need to be lightweight as
 * they will be called every time the cursor is moved. Alternative: design this
 * component to be reusable and add functionality to update tiles.
 */
typedef struct _UITileInfo UITileInfo;

/*
 * Creates a new tile info panel UI component.
 */
UITileInfo *ui_tile_info_new(World *w, UIRect dim);

/*
 * Draws the provided tile info panel with information in tile with given index.
 *
 * Before drawing all the values displayed are updated by calling getters on
 * the World entity.
 */
void ui_tile_info_draw(UITileInfo *ti, int tile_index);

/*
 * Refreshes just one line of the tile info panel.
 * Lines indexes as follows:
 * Index Field
 * 0     Visible or not NOTE: Isn't this completely useless?
 * 1     Number of enemies or hp
 * 2     Resources per turn
 * 3     Remaining resources
 */
void ui_tile_info_print_single_line(UITileInfo *ti, int tile_index, int line_index);

/*
 * Frees all the memory associated with a UITileInfo component that was
 * allocated by ui_tile_info_new.
 */
void ui_tile_info_destroy(UITileInfo *ti);

#endif
