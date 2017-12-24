#ifndef __UI_TILE_INFO__
#define __UI_TILE_INFO__

#include "entities/world.h"

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
UITileInfo *ui_tile_info_new(World *w);

/*
 * Draws a new tile info panel starting from (x,y).
 *
 * Before drawing all the values displayed are updated by calling getters on
 * the World entity.
 */
void ui_tile_info_draw(UITileInfo *ti, int tile_index, int x, int y);

/*
 * Frees all the memory associated with a UITileInfo component that was
 * allocated by ui_tile_info_new.
 */
void ui_tile_info_destroy(UITileInfo *ti);

#endif
