#include "../ui-map.h"

#include "../lib/error_handling.h"
#include "../lib/config.h"

/*
 * UIMap
 *
 * A Map view displayed on the center of the screen.
 */

struct _UIMap {
    Tile **tiles;

    int previous_cursor;
    int cursor;

    int height;
    int num_tiles;
};

UIMap *ui_map_new(World *w)
{
    if (!w) {
        HE("invalid arguments", "ui_map_new");
        return NULL;
    }

    UIMap *m = oopsalloc(1, sizeof(UIMap), "ui_map_new");

    m->tiles = world_get_tiles(w);
    if (!m->tiles) {
        HE("could not get tiles from given world", "ui_map_new");
        free(m);
        return NULL;
    }

    m->num_tiles = world_get_num_tiles(w);
    m->height = config_get_int("map height");
    if (m->height == 0) {
        HE("invalid height, cannot be 0", "ui_map_new");
        return NULL;
    }

    m->cursor = config_get_int("initial cursor");

    return m;
}

int ui_map_move_cursor(UIMap *m, UIMapVector dir)
{
    if (!m) {
        HE("invalid arguments", "ui_move_cursor");
        return UINT_ERROR;
    }

    switch (dir) {
        case UP:
            if (m->cursor % m->height > 0)
                ui_map_update_cursor(m, m->cursor - 1);
            else
                return UINT_ERROR;
            break;
        case LEFT:
            if (m->cursor - m->height >= 0)
                ui_map_update_cursor(m, m->cursor - m->height);
            else
                return UINT_ERROR;
            break;
        case DOWN:
            if (m->cursor % m->height != m->height - 1)
                ui_map_update_cursor(m, m->cursor + 1);
            else
                return UINT_ERROR;
            break;
        case RIGHT:
            if (m->cursor + m->height < m->num_tiles)
                ui_map_update_cursor(m, m->cursor + m->height);
            else
                return UINT_ERROR;
            break;
        default:
            printf("That direction is not supported.\n");
            return UINT_ERROR;
    }

    return !UINT_ERROR;
}

void ui_map_update_cursor(UIMap *m, int cursor)
{
    if (!m) {
        HE("invalid arguments", "ui_map_update_cursor");
        return;
    }

    m->previous_cursor = m->cursor;
    m->cursor = cursor;
}

int ui_map_get_cursor(UIMap *m)
{
    if (!m) {
        HE("invalid arguments", "ui_map_update_cursor");
        return UINT_ERROR;
    }

    return m->cursor;
}

void ui_map_redraw_tile(UIMap *m, int tile_index)
{
    // TODO
    return;
}

void ui_map_destroy(UIMap *m)
{
    free(m);
    return;
}
