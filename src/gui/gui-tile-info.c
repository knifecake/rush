#include "../ui.h"

#include "../lib/error_handling.h"
#include "../lib/config.h"

struct _UITileInfo {
    World *w;

    // rendering parameters
    int x, y;
    int width, height;

    // internally we use a text panel at the moment
    UITextPanel *tp;
};


UITileInfo *ui_tile_info_new(World *w, UIRect dim)
{
    if (!w) {
        HE("invalid arguments", "ui_tile_info_new");
        return NULL;
    }

    UITileInfo *ti = oopsalloc(1, sizeof(UITileInfo), "ui_tile_info_new");

    ti->width = dim.width;
    ti->height = dim.height;
    ti->x = dim.x;
    ti->y = dim.y;

    ti->w = w;

    ti->tp = ui_text_panel_new(ti->x, ti->y, ti->width, ti->height, ui_get_font());

    return ti;
}

void ui_tile_info_draw(UITileInfo *ti, int tile_index)
{
    if (!ti || tile_index < 0) {
        HE("invalid arguments", "ui_tile_info_draw");
        return;
    }

    char buff[100];
    Building *b = tile_get_building(world_tile_at_index(ti->w, tile_index));
    if (!b)
        sprintf(buff, "Tile %d\nPress b to build.", tile_index);
    else
        sprintf(buff, "Tile %d\n%s", tile_index, building_get_sprite(b));

    ui_text_panel_print(ti->tp, buff);
}

void ui_tile_info_destroy(UITileInfo *ti)
{
    if (!ti)
        return;

    free(ti);
}
