#include "ui.h"

#include "lib/error_handling.h"

#include <stdlib.h>
#include <stdio.h>

#define XLEN 68
#define YLEN 40
#define NT 8

/* Private functions */
int _coordinates_by_index_ (int index, int *x, int *y);

int _relative_coordinates_(int index, int first_index);
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
    // TODO
    return NULL;
}

void ui_world_info_draw(UIWorldInfo *wi, int x, int y)
{
    // TODO
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

int _coordinates_by_index_ (int index, int *x, int *y){
  if(index < 0){
    HE("index is negative", "_coordinates_by_index_";)
    return UINT_ERROR;
  }
  if(index%NT < NT/2){
    *x = 1 + (index/NT) * XLEN;
    *y = 1 + (YLEN/2) + YLEN * (index%NT);
  }else{
    *x = 1 + (XLEN/2) + (index/NT) * XLEN;
    *y = 1 + YLEN * ((index%NT) - (NT/2));
  }
  return !UINT_ERROR;
};

int _relative_coordinates_ (int index, int first_index){
  if(index < 0 || first_index < 0){
    HE("index is negative", "_relative_coordinates_");
    return UINT_ERROR;
  }
  return (index - first_index)+((index - first_index)/world_get_heigth(ui->w)) * (NT/2 - world_get_heigth(ui->w));
}
