#include "map.h"
#include "../lib/error_handling.h"
#include "../lib/config.h"
#include <assert.h>

struct _Map {
    Tile **tiles;
    int num_tiles;

    Tile **map;
    int map_tiles;
};

/*
 * TODO: IMPORTANT, sergio si esto no es tuyo tenemos que atribuirlo.
 * Also, if this is private, by convention it should be named _aleat_num;
 */

/*
 * Private functions
 */

  /*
   * In these functions neighbour is a number in the range [0,5].
   * They are the neighbours of the hex clockwise being 0 the top one.
   */
    int _tile_calc_neighbour(int tile_index, int neighbour, int h, int odd);
    int _tile_is_neighbour(Map *m, int tile_index, int neighbour, bool odd);
/*
 *
 */
int aleat_num(int inf, int sup){
  int n, r;
  long end;

  if(inf<sup){
    n=sup-inf+1;
  }
  else if(sup<inf){
    n=inf-sup+1;
  }
  else{
    fprintf(stderr, "aleat_num: cant generate a random number between the same number\n");
    return -1;
  }

  if ((n - 1) == RAND_MAX) {
      return rand();
  }
  else {
      /* Chop off all of the values that would cause skew...*/

      end = RAND_MAX / n; /* truncate skew*/
      assert (end > 0L);
      end *= n;

      /* ... and ignore results from rand() that fall above that limit.
       (Worst case the loop condition should succeed 50% of the time,
       so we can expect to bail out of this loop pretty quickly.)*/

      while ((r = rand()) >= end);

      r = r % n;
      if(inf<sup){
        r=inf+r;
      }
      else{
        r=sup+r;
      }
      return r;
    }
}

int map_gen_standard(Map *m, int map_tiles) {
    if (!m || map_tiles < 0) {
        HE("invalid arguments", "map_gen_standard");
        return UINT_ERROR;
    }

    if (m->num_tiles == 0 || !m->tiles) {
        HE("no tiles to generate map", "map_gen_standard");
        return UINT_ERROR;
    }

    // by convention, we null terminate list of entities, although it might not be necessary here
    m->map = oopsalloc(map_tiles + 1, sizeof(Tile *), "map_gen_standard");
    m->map_tiles = map_tiles;

    for (int i = 0; i < m->map_tiles; i++)
        m->map[i] = tile_copy(m->tiles[i % m->num_tiles]);

    return !UINT_ERROR;
}

int map_gen_random(Map *m, int map_tiles) {
    if (!m || map_tiles < 0) {
        HE("invalid arguments", "map_gen_standard");
        return UINT_ERROR;
    }

    if (m->num_tiles == 0 || !m->tiles) {
        HE("no tiles to generate map", "map_gen_standard");
        return UINT_ERROR;
    }

    // by convention, we null terminate list of entities, although it might not be necessary here
    m->map = oopsalloc(map_tiles + 1, sizeof(Tile *), "map_gen_standard");
    m->map_tiles = map_tiles;

    for (int i = 0; i < m->map_tiles; i++) {
      m->map[i] = tile_copy(m->tiles[aleat_num(0, m->num_tiles - 1)]);
    }

    return !UINT_ERROR;
}

Map *map_new(Tile **tiles, int num_tiles, layout_generation_function layout_gen, int map_tiles)
{
    if (!tiles || !layout_gen) {
        HE("invalid arguments", "map_new");
        return NULL;
    }

    Map *map = oopsalloc(1, sizeof(Map), "map_new");

    map->tiles = tiles;
    map->num_tiles = num_tiles;

    if (UINT_ERROR == layout_gen(map, map_tiles)) {
        HE("could not build map", "map_new");
        free(map);
        return NULL;
    }

    return map;
}

Tile *map_tile_at_index(Map *m, int tile_index)
{
    if (!m || tile_index < 0 || tile_index >= m->map_tiles) {
        HE("invalid arguments", "map_tile_at_index");
        return NULL;
    }

    return m->map[tile_index];
}

Tile **map_get_map_tiles(Map *m){
  if(!m){
    HE("Input error", "map_get_map_tiles")
    return NULL;
  }
  return m->map;
}

int *map_get_neighbour_tiles(Map *m, int tile_index){
  if(!m || tile_index < 0 || tile_index >= m->map_tiles ){
    HE("Input error", "map_get_neighbour_tiles")
    return NULL;
  }
  int height = config_get_int("map height");
  bool column_parity = (tile_index/height)%2;
  int *neighbours = oopsalloc(6, sizeof(int), "map_get_neighbour_tiles");
  neighbours[0] = _tile_is_neighbour(m, tile_index, 0, column_parity);
  neighbours[1] = _tile_is_neighbour(m, tile_index, 1, column_parity);
  neighbours[2] = _tile_is_neighbour(m, tile_index, 2, column_parity);
  neighbours[3] = _tile_is_neighbour(m, tile_index, 3, column_parity);
  neighbours[4] = _tile_is_neighbour(m, tile_index, 4, column_parity);
  neighbours[5] = _tile_is_neighbour(m, tile_index, 5, column_parity);
  return neighbours;
}

void map_destroy(Map *m)
{
    if (!m)
        return;

    for (int i = 0; i < m->map_tiles; tile_destroy(m->map[i++]));
    free(m);
}

int _tile_is_neighbour(Map *m, int tile_index, int neighbour, bool odd){
  if (!m || tile_index < 0){
    HE("Input error", "_tile_is_neighbour")
    return UINT_ERROR;
  }
  int height = config_get_int("map height");
  if(tile_index % height == 0){//First row
    if(neighbour == 0){
      return -1;
    }
    if(odd){
      if(neighbour == 1 || neighbour == 5){
        return -1;
      }
    }
  }
  if(tile_index % height == height - 1){//Last row
    if(neighbour == 3){
      return -1;
    }
    if(!odd){
      if(neighbour == 2 || neighbour == 4){
        return -1;
      }
    }
  }
  int neigh_index = _tile_calc_neighbour(tile_index, neighbour, height, odd);
  /*
   * Neighbour out of map (left, right, top-left, bottom-right)
   */
  if(neigh_index < 0 || neigh_index >= m->map_tiles){
    return -1;
  }
  return neigh_index;
}

int _tile_calc_neighbour(int tile_index, int neighbour, int h, int odd){
  switch (neighbour) {
    case 0:
      return tile_index - 1;
    case 1:
      if(odd)
        return tile_index + h - 1;
      return tile_index + h;
    case 2:
      if(odd)
        return tile_index + h;
      return tile_index + h + 1;
    case 3:
      return tile_index - 1;
    case 4:
      if(odd)
        return tile_index - h;
      return tile_index - h + 1;
    case 5:
      if(odd)
        return tile_index - h - 1;
      return tile_index - h;
    default:
      HE("Neighbour error", "_tile_is_neighbour")
      return UINT_ERROR;
  }
}
