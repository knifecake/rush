#include "ui.h"

#include "lib/error_handling.h"
#include "lib/config.h"
#include "lib/dict.h"
#include "lib/sprite.h"

#include <stdlib.h>
#include <stdio.h>

/* Private functions */
int _coordinates_by_index_ (int index, int *x, int *y);

int _relative_coordinates_(int index, int first_index);

int _get_map_info_from_dict(int *tiles_per_column, int *n_columns);

int _draw_sprite_in_index(UIMap *m, int index, char* sprite_name);
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
    int first_index;
    int previous_cursor;
};

UIMap *ui_map_new(World *w)
{
    // TODO
    return NULL;
}

void ui_map_update_cursor(UIMap *m, int cursor){
  if(!map || cursor < 0){
    HE("Input error", "ui_map_update_cursor")
    return;
  }
  int x, y, coord, tiles_per_column, n_columns, tiles_in_screen;

  if (UINT_ERROR == _draw_sprite_in_index(m, m->previous_cursor, "cursor_off"){
    HE("Error drawing sprite in given index", "ui_map_update_cursor");
    return;
  }

  if (UINT_ERROR == _draw_sprite_in_index(m, cursor, "cursor_on"){
    HE("Error drawing sprite in given index", "ui_map_update_cursor");
    return;
  }

  m->previous_cursor = cursor;
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

int _coordinates_by_index (int index, int *x, int *y){
  if(index < 0){
    HE("index is negative", "_coordinates_by_index");
    return UINT_ERROR;
  }
  /* This chunk will only get data from config dictionary */
  char *handle;
  if(handle = config_get("tiles_per_column")){
    HE("Error retrieving tiles_per_column from config dictionary")
    return UINT_ERROR;
  }
  int num_tiles = atoi(config_get(handle));
  if(handle = config_get("hex_xlen")){
    HE("Error retrieving tiles_per_column from config dictionary")
    return UINT_ERROR;
  }
  int xlen = atoi(config_get(handle));
  if(handle = config_get("hex_ylen")){
    HE("Error retrieving tiles_per_column from config dictionary")
    return UINT_ERROR;
  }
  int ylen = atoi(config_get(handle));
  if(handle = config_get("hex_init")){
    HE("Error retrieving tiles_per_column from config dictionary")
    return UINT_ERROR;
  }
  int init_x = atoi(config_get(handle));
  if(handle = config_get("hex_init_y")){
    HE("Error retrieving tiles_per_column from config dictionary")
    return UINT_ERROR;
  }
  int init_y = atoi(config_get(handle));
  /*******************************************/
  if(index%num_tiles < num_tiles/2){
    *x = init_x + (index/num_tiles) * xlen;
    *y = init_y + (ylen/2) + ylen * (index%num_tiles);
  }else{
    *x = init_x + (xlen/2) + (index/num_tiles) * xlen;
    *y = init_y + ylen * ((index%num_tiles) - (num_tiles/2));
  }
  return !UINT_ERROR;
};

int _relative_coordinates (int index, int first_index){
  if(index < 0 || first_index < 0){
    HE("index or first index is negative", "_relative_coordinates");
    return UINT_ERROR;
  }
  char *handle;
  if(handle = config_get("tiles_per_column")){
    HE("Error retrieving tiles_per_column from config dictionary")
    return UINT_ERROR;
  }
  int num_tiles = atoi(config_get(handle));
  return (index - first_index)+((index - first_index)/world_get_heigth(ui.w)) * (num_tiles/2 - world_get_heigth(ui.w));
}


int _get_map_info_from_dict(int *tiles_per_column, int *n_columns){
  char *handle;
  handle = config_get("tiles_per_column");
  if(!handle){
    HE("Error getting tiles_per_column value", "_get_map_info_from_dict");
    return UINT_ERROR;
  }
  *tiles_per_column = atoi(handle);

  handle = config_get("columns_in_screen");
  if(!handle){
    HE("Error getting columns_in_screen value", "_get_map_info_from_dict");
    return UINT_ERROR;
  }
  *n_columns = atoi(handle);
  return !UINT_ERROR;
}

int _draw_sprite_in_index(UIMap *m, int index, char* sprite_name){
  int tiles_per_column, n_columns, tiles_in_screen, coord;

  if (UINT_ERROR == _get_map_info_from_dict(&tiles_per_column, &n_columns)){
    HE("Error retrieving map info from config dictionary", "_draw_sprite_in_index")
    return UINT_ERROR;
  }
  tiles_in_screen = tiles_per_column * n_columns;

  coord = _relative_coordinates(m->first_index, index);
  if (coord = UINT_ERROR || coord < 0 || coord >= tiles_in_screen){
    HE("Coordinates out of map view", "_draw_sprite_in_index")
    return UINT_ERROR;
  }
  if(UINT_ERROR == _coordinates_by_index(coord, &x, &y)){
    HE("Error while calculating index Coordinates", "_draw_sprite_in_index")
    return UINT_ERROR;
  };

  Sprite* s;
  s = dict_get(sprite_dict, sprite_name);
  if(!s){
    HE("Error retrieving sprite data from sprite dictionary", "_draw_sprite_in_index");
    return UINT_ERROR;
  }
  sprite_draw(fp, s, x, y);
  return !UINT_ERROR;
}
