#include "../lib/sprite.h"
#include "../lib/dict.h"

/*
 * This function will allocate a dictionary string-sprite that will keep
 * the memory of all the sprites in the game.
 * Sprites should always be loaded from this function and accessed through
 * the dictionary.
 */
Dict *load_sprite_dict_from_file(char *);
