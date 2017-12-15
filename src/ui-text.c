#include "ui.h"

#include "lib/error_handling.h"

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
    ui.wi = ui_world_info_new(ui.w);

    // ui tile info is created when the cursor is first moved

    return !UINT_ERROR;
}

void ui_teardown()
{
    // this function does nothing at the moment as the UI is
    // statically allocated
    return;
}

int ui_update_cursor()
{
  // TODO
  return UINT_ERROR;
}

int ui_redraw_tile(int tile_index)
{
  return UINT_ERROR;
  // TODO
}

int ui_update_world_info()
{
  ui_world_info_draw(ui.wi, 0, 0);
  return UINT_ERROR;
}

// No need to call this after calling update cursor, it gets called automatically.
int ui_update_tile_info();


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

/*
 * UIWorldInfo panel
 *
 * A world info panel intended to be displayed on the top of the sidebar.
 */

struct _UIWorldInfo {
    // TODO
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
    printf("hello\n");
    world_print(stdout, wi->w);
    return;
}

void ui_world_info_destroy(UIWorldInfo *wi)
{
    // TODO
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
    // TODO
    Tile *w;
};

UITileInfo *ui_tile_info_new(Tile *t)
{
    // TODO
    return NULL;
}

void ui_tile_info_draw(UITileInfo *ti, int x, int y)
{
    // TODO
    return;
}

void ui_tile_info_destroy(UITileInfo *ti)
{
    // TODO
    return;
}

// ----------------------------------------------------------------------------

/*
 * UIMap
 *
 * A Map view displayed on the center of the screen.
 */

struct _UIMap {
    // TODO
    Tile **tiles;
    int previous_cursor;
};

UIMap *ui_map_new(World *w)
{
    // TODO
    return NULL;
}

void ui_map_update_cursor(UIMap *m)
{
    // TODO
    return;
}

void ui_map_redraw_tile(UIMap *m, int tile_index)
{
    // TODO
    return;
}

void ui_map_destroy(UIMap *m)
{
    // TODO
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

    UIList *l = calloc(1, sizeof(UIList));
    if (!l) {
        HE("could not allocate memory", "ui_list_new");
        return NULL;
    }

    l->list = calloc(s_len, sizeof(UIListItem *));
    if (!l->list) {
        HE("could not allocate memory for list", "ui_list_new");
        free(l);
        return NULL;
    }

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

    printf("Should present a list");
    return l->list[0];
}
