#include "../ui.h"

#include "../lib/error_handling.h"
#include "../lib/config.h"
#include "../lib/sprite.h"
#include "../lib/dict.h"

#include <string.h>

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

/* Private functions */
int _coordinates_by_index_ (UIMap *m, int index, int *x, int *y);

int _relative_coordinates_(UIMap *m, int index);

int _draw_sprite_in_index(UIMap *m, int index, char* sprite_name);

int _center_screen_index(UIMap *m, int central_index);

void _move(UIMap *m, UIMapVector dir, UIMapVector edge1, UIMapVector edge2);

int _calculate_cursor(int cursor, UIMapVector dir, int height);

void _draw_map(UIMap *m);

void _calculate_edge (UIMap *m, UIMapVector *edge1, UIMapVector *edge2, int height, int n_columns, bool real);

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

  m->tiles = map_get_map_tiles(world_get_map(w));
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
  _calculate_edge(m, &true_edge1, &true_edge2, m->true_height, m->true_n_columns, true);
  if(dir == true_edge1 || dir == true_edge2){
    return UINT_ERROR;
  }
  _calculate_edge(m, &rel_edge1, &rel_edge2, m->twice_screen_height/2, m->screen_columns, false);
  _move(m, dir, rel_edge1, rel_edge2);
  ui_update_tile_info();
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

  Sprite *background;
  background = dict_get(ui_get_sprite_dict(), "background");
  sprite_draw(stdout, background, 0, 0);

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
  s = dict_get(ui_get_sprite_dict(), sprite_name);
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
    if(dir == LEFT || dir == RIGHT){
      m->first_index = _calculate_cursor(m->first_index, dir, m->true_height);
    }
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

void _calculate_edge (UIMap *m, UIMapVector *edge1, UIMapVector *edge2, int height, int n_columns, bool real){
  if(!m || !edge1 || !edge2){
    HE("Map is null", "_calculate_edge")
    return;
  }
  int current_cursor;
  if(real){
    current_cursor=m->previous_cursor;
  }else{
    current_cursor=_relative_coordinates(m, m->previous_cursor);
  }
  *edge1 = *edge2 = HERE;
  if(current_cursor % height == 0){
    *edge1 = UP;
  }
  if(current_cursor % height == height-1){
    *edge1 = DOWN;
  }
  if(current_cursor / height == 0){
    if(*edge1 == HERE){
      *edge1 = LEFT;
    }else{
      *edge2 = LEFT;
    }
  }
  if(current_cursor / height == n_columns - 1){
    if(*edge1 == HERE){
      *edge1 = RIGHT;
    }else{
      *edge2 = RIGHT;
    }
  }
  return;
}
