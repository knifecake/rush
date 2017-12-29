#include "../ui.h"

#include "../lib/error_handling.h"
#include "../lib/config.h"

struct _UITileInfo {
    World *w;

    // rendering parameters
    UIRect dim;

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

    ti->dim = dim;
    ti->w = w;

    ti->tp = ui_text_panel_new(ti->dim, ui_get_font());

    return ti;
}

void ui_tile_info_draw(UITileInfo *ti, int tile_index)
{
    if (!ti || tile_index < 0) {
        HE("invalid arguments", "ui_tile_info_draw");
        return;
    }

    Tile *tile = world_tile_at_index(ti->w, tile_index);
    if (!tile) {
        HE("could not retrieve tile", "ui_tile_info_draw");
    }

    char *info = oopsalloc(MAX_RESOURCES * 10 + 100, sizeof(char), "ui_tile_info_draw");

    // communicate the presence of a building // TODO: remove this when building sprites are done
    Building *b = tile_get_building(tile);
    if (!b)
        sprintf(info, "Tile %d\nPress b to build.\n", tile_index);
    else
        sprintf(info, "Tile %d\n%s (%d)\n", tile_index, building_get_sprite(b), building_get_level(b));

    for (int i = 0; i < MAX_RESOURCES; i++)
        sprintf(info, "%s%d: %d\n", info, i, tile_get_remaining_resources(tile, i));

    ui_text_panel_print(ti->tp, info);
}

void ui_tile_info_destroy(UITileInfo *ti)
{
    if (!ti)
        return;

    free(ti->tp);
    free(ti);
}
