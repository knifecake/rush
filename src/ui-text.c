#include "ui.h"

#include "lib/error_handling.h"
#include "lib/terminal.h"
#include "lib/config.h"

#include <stdlib.h>
#include <stdio.h>


/*
 * The global UI structure.
 */
typedef struct _UI {
    World *w;

    UIMap *map;
    UIWorldInfo *wi;
    UITileInfo *ti;
} UI;

UI ui;

int ui_setup(World *w)
{
    if (!w) {
        HE("invalid paramenters, no world given", "ui_setup");
        return UINT_ERROR;
    }

    ui.w = w;

    ui.map = ui_map_new(ui.w);
    if (!ui.map) {
        HE("could not load map", "ui_setup");
        return UINT_ERROR;
    }

    ui.wi = ui_world_info_new(ui.w);
    ui.ti = ui_tile_info_new(ui.w);

    // ui tile info is created when the cursor is first moved

    return !UINT_ERROR;
}

void ui_teardown()
{
    // this function does nothing at the moment as the UI is
    // statically allocated
    return;
}

UIMapVector _ui_keypress_to_vector(int input)
{
    switch (input) {
        case UP_ARROW:
            return UP;
        case LEFT_ARROW:
            return LEFT;
        case DOWN_ARROW:
            return DOWN;
        case RIGHT_ARROW:
            return RIGHT;
        default:
            return HERE;
    }
}

int ui_move_cursor(int input)
{
    return ui_map_move_cursor(ui.map, _ui_keypress_to_vector(input));
}

int ui_get_cursor()
{
    return ui_map_get_cursor(ui.map);
}

int ui_redraw_tile(int tile_index)
{
  return UINT_ERROR;
  // TODO
}

int ui_update_world_info()
{
  ui_world_info_draw(ui.wi, 0, 0);
  return !UINT_ERROR;
}

// No need to call this after calling update cursor, it gets called automatically.
int ui_update_tile_info()
{
    ui_tile_info_draw(ui.ti, ui_map_get_cursor(ui.map), 0, 0);
    return !UINT_ERROR;
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

/*
 * UIWorldInfo panel
 *
 * A world info panel intended to be displayed on the top of the sidebar.
 */

struct _UIWorldInfo {
    World *w;
};

UIWorldInfo *ui_world_info_new(World *w)
{
    if (!w) {
        HE("invalid parameters", "ui_world_info_new");
        return NULL;
    }

    UIWorldInfo *wi = oopsalloc(1, sizeof(UIWorldInfo), "ui_world_info_new");
    wi->w = w;
    return wi;
}

void ui_world_info_draw(UIWorldInfo *wi, int x, int y)
{
    if (!wi) {
        HE("invalid params", "ui_world_info_draw");
        return;
    }

    printf("You have the following resources:\n");
    Resource **res = world_get_resources(wi->w);
    if (!res) {
        HE("cannot print get resources from world", "ui_world_info_draw");
        return;
    }

    for (int i = 0; i < world_get_num_resources(wi->w); i++) {
        printf("%s: %d\n", resource_get_name(res[i]), world_get_resource_quantity(wi->w, i));
    }

    printf("\n");
}

void ui_world_info_destroy(UIWorldInfo *wi)
{
    free(wi);
    return;
}

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct _UIListItem {
    void *info;
} UIListItem;

struct _UIList {
    UIListItem **list;
    int len;
    ui_get_li_string_fun get_li_title;
};

UIListItem *_ui_li_new(void *info)
{
    if (!info) {
        HE("invalid params", "_ui_li_new");
        return NULL;
    }

    UIListItem *li = calloc(1, sizeof(UIListItem));
    if (!li) {
        HE("could not allocate memory for list item", "_ui_li_new");
        return NULL;
    }

    li->info = info;
    return li;
}

void _ui_li_destroy(UIListItem *li)
{
    if (!li) return;

    free(li);
}

UIList *ui_list_new(void **s, int s_len,
        ui_get_li_string_fun get_li_title,
        ui_get_li_sprite_fun get_li_sprite)
{
    if (!s || s_len < 0 || !get_li_title) {
        HE("invalid parameters", "ui_list_new");
        return NULL;
    }

    UIList *l = oopsalloc(1, sizeof(UIList), "ui_list_new");

    l->list = oopsalloc(s_len, sizeof(UIListItem *), "ui_list_new");

    for (int i = 0; i < s_len; i++) {
        l->list[i] = _ui_li_new(s[i]);
        if (!l->list[i]) {
            HE("could not create list item", "ui_list_new");
            for (int j = 0; j < i; _ui_li_destroy(l->list[j++]));
            free(l->list); free(l);
            return NULL;
        }
    }

    l->len = s_len;

    l->get_li_title = get_li_title;

    return l;
}

void ui_list_destroy(UIList *l)
{
    if (!l) return;

    for (int i = 0; i < l->len; _ui_li_destroy(l->list[i++]));
    free(l->list);
    free(l);
}

// TODO: finish implementation
void *ui_list_present(UIList *l)
{
    if (!l) {
        HE("invalid parameters", "ui_list_present");
        return NULL;
    }

    for (int i = 0; i < l->len; i++) {
        printf("[%2d]: %s\n", i + 1, l->get_li_title(l->list[i]->info));
    }


    int index = -1;
    do {
        printf("Choose from the list by typing a number or type 'q' to exit without choosing: ");
        char *buff = term_read_string(stdin);

        if (!buff) {
            HE("could not read from stdin", "ui_list_present");
            return NULL;
        }

        if (buff[0] == 'q' || buff[0] == 'Q') {
            printf("Selected nothing.\n");
            return NULL;
        }

        index = atoi(buff);
    } while (index <= 0 || index > l->len);

    return l->list[index - 1]->info;
}
