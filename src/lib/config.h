#ifndef __CONFIG_H__
#define __CONFIG_H__

/*
 * Configuration storage module.
 *
 * This module provides a common interface to store general settings about the
 * game. Configuration items can be stored dynamically or be loaded from a
 * file. All configuration options are available throughout the game, to every
 * entity and every lib file, so take care of avoiding key collisions, for
 * example by prefixing your keys.
 */

/*
 * Stores a key-value pair in the config dictionary. Returns UINT_ERROR on
 * error. If the given key was already stored, it's value is overriden by the
 * new one.
 */
int config_set(char *key, char *value);


/*
 * Returns the value associated with the given key, NULL on error.
 */
char *config_get(char *key);

/*
 * Loads configuration from the given filename. Returns UINT_ERROR on error.
 */
int load_config_from_file(const char *config_file);


/*
 * Frees all resources allocated by the config module.
 */
void config_destroy();
#endif
