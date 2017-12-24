#include "ui.h"
#include "entities/building.h"
#include "lib/error_handling.h"
#include "lib/config.h"
#include "lib/dict.h"
#include "lib/sprite.h"
#include "lib/terminal.h"
#include "asset_loaders/sprite_repository.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Private functions */
int _coordinates_by_index_ (UIMap *m, int index, int *x, int *y);

int _relative_coordinates_(UIMap *m, int index);

int _draw_sprite_in_index(UIMap *m, int index, char* sprite_name);

int _center_screen_index(UIMap *m, int central_index);

void _move(UIMap *m, UIMapVector dir, UIMapVector edge1, UIMapVector edge2);

int _calculate_cursor(int cursor, UIMapVector dir, int height);

void _draw_map(UIMap *m);

void _calculate_edge (UIMap *m, UIMapVector *edge1, UIMapVector *edge2, int height, int n_columns);
/*
 * Global variables
 */
 static Dict* sprite_dict;

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

    sprite_dict = load_sprite_dict_from_file(config_get("sprite db"));
    if(!sprite_dict){
      HE("Error creating the sprite dictionary", "ui_setup")
      return UINT_ERROR;
    }
    // ui tile info is created when the cursor is first moved
    ui_map_draw(ui.map);
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

int ui_redraw_tile(int tile_index){
  /*TODO: Implement this*/
  return !UINT_ERROR;
}

int ui_update_world_info(){
  /*TODO: Implement this*/
  return !UINT_ERROR;
}

// No need to call this after calling update cursor, it gets called automatically.
int ui_update_tile_info(){
  /*TODO: Implement this*/
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

UITileInfo *ui_tile_info_new(World* w)
{
    // TODO
    return NULL;
}

void ui_tile_info_draw(UITileInfo *ti, int tile_index, int x, int y)
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
    Tile **tiles;

    int first_index;
    int previous_cursor;

    int true_n_columns;
    int true_height;

    int screen_tiles; /* Tiles visibles in screen */
    int twice_screen_height; /* Twice the number of tiles in one screen column */
    int screen_columns; /*Screen columns*/
};


UIMap *ui_map_new(World *w){
  if(!w){
    HE("Input error", "ui_map_new")
    return NULL;
  }
  UIMap* m = oopsalloc(1, sizeof(UIMap), "ui_map_new");

  m->previous_cursor = config_get_int("initial cursor");

  m->first_index = _center_screen_index(m, m->previous_cursor);
  if(m->first_index < 0){
    HE("Error retrieving index for first tile in screen", "ui_map_new")
    return NULL;
  }

  m->tiles = world_get_map(ui.w);
  if(!m->tiles){
    HE("Error retrieving tile list from world", "ui_map_new")
    return NULL;
  }
  m->true_n_columns = config_get_int("map columns");
  m->true_height = config_get_int("map height");
  m->twice_screen_height = config_get_int("tiles_per_double_column");
  m->screen_columns = config_get_int("columns_in_screen");
  m->screen_tiles = m->screen_columns * m->twice_screen_height/2;

  m->first_index = _center_screen_index(m, m->previous_cursor);
  if(m->first_index < 0){
    HE("Error retrieving index for first tile in screen", "ui_map_new")
    return NULL;
  }
  return m;
}

int ui_map_move_cursor(UIMap *m, UIMapVector dir){
  if(!m || dir < UP || dir > HERE){
    HE("Input error", "ui_map_update_cursor");
    return UINT_ERROR;
  }

  UIMapVector true_edge1, true_edge2, rel_edge1, rel_edge2;
  _calculate_edge(m, &true_edge1, &true_edge2, m->true_height, m->true_n_columns);
  if(dir == true_edge1 || dir == true_edge2){
    return UINT_ERROR;
  }
  _calculate_edge(m, &rel_edge1, &rel_edge2, m->twice_screen_height/2, m->screen_columns);
  _move(m, dir, rel_edge1, rel_edge2);
  return !UINT_ERROR;
}

void ui_map_redraw_tile(UIMap *m, int tile_index){
  if(!m || tile_index < 0){
    HE("Input error", "ui_map_redraw_tile")
    return;
  }
  if (UINT_ERROR == _draw_sprite_in_index(m, tile_index, tile_get_sprite(m->tiles[tile_index]))){
    HE("Error drawing tile given the index", "ui_map_redraw_tile")
    return;
  }
  Building* b;
  if( (b = tile_get_building(m->tiles[tile_index])) ){
    if (UINT_ERROR == _draw_sprite_in_index(m, tile_index, building_get_sprite(b))){
      HE("Error drawing building in the given index", "ui_map_redraw_tile")
      return;
    }
  }
  return;
}

int ui_map_get_cursor(UIMap *m)
{
    if (!m) {
        HE("invalid arguments", "ui_map_update_cursor");
        return UINT_ERROR;
    }

    return m->previous_cursor;
}

void ui_map_draw(UIMap *m){
  if(!m){
    HE("Map is null", "ui_map_draw")
    return;
  }
  _draw_map(m);
  if (UINT_ERROR == _draw_sprite_in_index(m, m->previous_cursor, "cursor_on")){
    HE("Error drawing sprite in given index", "ui_map_update_cursor");
    return;
  }
}

void ui_map_destroy(UIMap *m)
{
    free(m);
    return;
}

int _coordinates_by_index (UIMap* m, int index, int *x, int *y){
  if(index < 0){
    HE("index is negative", "_coordinates_by_index");
    return UINT_ERROR;
  }
  /*
   * This chunk will only get data from config dictionary
   */
  int num_tiles = m->twice_screen_height;
  int xlen = config_get_int("hex_xlen");
  int ylen = config_get_int("hex_ylen");
  int init_x = config_get_int("hex_init_x");
  int init_y = config_get_int("hex_init_y");
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

int _relative_coordinates (UIMap* m, int index){
  if(index < 0 || !m){
    HE("index or first index is negative", "_relative_coordinates")
    return UINT_ERROR;
  }
  int first_index = m->first_index;
  int num_tiles = m->twice_screen_height;
  int height = m->true_height;
  return (index - first_index)+((index - first_index)/height) * (num_tiles/2 - height);
}

int _draw_sprite_in_index(UIMap *m, int index, char* sprite_name){
  int tiles_in_screen, coord, x, y;
  FILE *fp = stdout;

  tiles_in_screen = m->screen_tiles;

  coord = _relative_coordinates(m, index);
  if ((coord == UINT_ERROR) || coord < 0 || coord >= tiles_in_screen){
    HE("Coordinates out of map view", "_draw_sprite_in_index")
    return UINT_ERROR;
  }
  if(UINT_ERROR == _coordinates_by_index(m, coord, &x, &y)){
    HE("Error while calculating index Coordinates", "_draw_sprite_in_index")
    return UINT_ERROR;
  };

  Sprite* s;
  s = dict_get(sprite_dict, sprite_name);
  if(!s){
    HE("Error retrieving sprite data from sprite dictionary", "_draw_sprite_in_index");
    return UINT_ERROR;
  }
  if(strcmp(sprite_name, "cursor_on") == 0 || strcmp(sprite_name, "cursor_off") == 0){
    x -= 3;
    y -= 3;
  }
  sprite_draw(fp, s, x, y);
  return !UINT_ERROR;
}

int _center_screen_index(UIMap* m, int central_index){
  if(central_index < 0){
    HE("index cannot be negative", "_center_screen_index")
    return UINT_ERROR;
  }

  int tiles_per_double_column = m->twice_screen_height;
  int n_columns = m->screen_columns;
  int height = m->true_height;

  return central_index - height * (n_columns/2) - (tiles_per_double_column/4);
}

void _move(UIMap *m, UIMapVector dir, UIMapVector edge1, UIMapVector edge2){
  if (UINT_ERROR == _draw_sprite_in_index(m, m->previous_cursor, "cursor_off")){
    HE("Error drawing sprite in given index", "ui_map_update_cursor");
    return;
  }

  if (dir == edge1 || dir == edge2){
    m->first_index = _calculate_cursor(m->first_index, dir, m->true_height);

    _draw_map(m);

  }
  m->previous_cursor = _calculate_cursor(m->previous_cursor, dir, m->true_height);

  if (UINT_ERROR == _draw_sprite_in_index(m, m->previous_cursor, "cursor_on")){
    HE("Error drawing sprite in given index", "ui_map_update_cursor");
    return;
  }
}

int _calculate_cursor(int cursor, UIMapVector dir, int height){
  switch (dir) {
    case UP:
      return cursor -= 1;
    case LEFT:
      return cursor -= height;
    case DOWN:
      return cursor += 1;
    case RIGHT:
      return cursor += height;
    default:
      HE("Something weird happened", "_calculate_cursor")
      return UINT_ERROR;
  }
}

void _draw_map(UIMap *m){
  if (!m){
    HE("Map is null", "_draw_map")
    return;
  }
  int screen_tiles = m->twice_screen_height/2;
  int initial_tile = m->first_index;

  Sprite *background;
  background = dict_get(sprite_dict, "background");
  sprite_draw(stdout, background, 0, 0);

  for(int i = m->first_index, j=0, count=0; count < m->screen_tiles; count++){
    ui_map_redraw_tile(m, i);
    if(j == screen_tiles - 1){
      initial_tile += m->true_height;
      i = initial_tile;
      j = 0;
    }else{
      ++i;
      ++j;
    }
  }
}

void _calculate_edge (UIMap *m, UIMapVector *edge1, UIMapVector *edge2, int height, int n_columns){
  if(!m || !edge1 || !edge2){
    HE("Map is null", "_calculate_edge")
    return;
  }
  *edge1 = *edge2 = HERE;
  if(m->previous_cursor % height == 0){
    *edge1 = UP;
  }
  if(m->previous_cursor % height == height-1){
    *edge1 = DOWN;
  }
  if(m->previous_cursor / height == 0){
    if(*edge1 == HERE){
      *edge1 = LEFT;
    }else{
      *edge2 = LEFT;
    }
  }
  if(m->previous_cursor / height == n_columns - 1){
    if(*edge1 == HERE){
      *edge1 = RIGHT;
    }else{
      *edge2 = RIGHT;
    }
  }
  return;
}
