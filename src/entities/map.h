#ifndef __MAP_H__
#define __MAP_H__

/**
 * Map
 *
 * An abstraction for a list of tiles (that may be different from the tiles
 * avaiable in the game loaded by tile_loader).
 */
typedef struct _Map Map;

#include "tile.h"

/*
 * Un prototipo para una función que coge un mapa y dispone las tiles de alguna forma (aleatoriamente
 * o no, dependiendo de la función implementada). Estas funciones deberían hacer copia de las tiles
 * que pasamos a map_new. De esta manera, no es necesario que map guarde una lista con las tiles
 * plantilla (en el caso de que la función no sea aleatoria, el propio mapa será la lista de las tiles
 * plantilla).
 */
typedef int (*layout_generation_function)(Map *m, int map_tiles);

/*
 * A list of map generator functions follows:
 */


/*
 * Does not introduce randomness, just copies tiles from the list of canonical
 * tiles. If map_tiles is greater than the number of available tiles it loops
 * around copying the first tiles in the list again and so on.
 */
int map_gen_standard(Map *m, int map_tiles);

int map_gen_random(Map *m, int map_tiles);


/*
 * The methods for this entity follow:
 */
Map *map_new(Tile **tiles, int num_tiles, layout_generation_function layout_gen, int map_tiles);

Tile *map_tile_at_index(Map *m, int tile_index);

Tile **map_get_map_tiles(Map *m);

void map_destroy(Map *m);

#endif
