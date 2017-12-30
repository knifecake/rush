#include "map.h"
#include "../lib/error_handling.h"

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

void map_destroy(Map *m)
{
    if (!m)
        return;

    for (int i = 0; i < m->map_tiles; tile_destroy(m->map[i++]));
    free(m);
}
