#include "../ui-tile-info.h"

#include "../lib/error_handling.h"

/*
 * UITileInfo panel
 *
 * A tile info panel intended to be displayed on the right of the sidebar.
 *
 * Notice: this component's new/destroy functions need to be lightweight as
 * they will be called every time the cursor is moved. Alternative: design this
 * component to be reusable and add functionality to update tiles.
 */

struct _UITileInfo {
    World *w;
};

UITileInfo *ui_tile_info_new(World *w)
{
    if (!w) {
        HE("invalid arguments", "ui_tile_info_new");
        return NULL;
    }

    UITileInfo *ti = oopsalloc(1, sizeof(UITileInfo), "ui_tile_info_new");
    ti->w = w;
    return ti;
}

void ui_tile_info_draw(UITileInfo *ti, int tile_index, int x, int y)
{
    if (!ti || tile_index < 0) {
        HE("invalid arguments", "ui_tile_info_draw");
        return;
    }

    printf("You are at tile %d\n", tile_index);

    Building *b;
    if (!(b = tile_get_building(world_tile_at_index(ti->w, tile_index))))
        printf("There is nothing built on this tile. To build something press b.\n");
    else
        printf("There is a building on this tile: %s\n", building_get_sprite(b));
}

void ui_tile_info_destroy(UITileInfo *ti)
{
    free(ti);
}
