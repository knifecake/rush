/*
 * Configuration storage module.
 *
 * This module provides a common interface to store general settings about the
 * game. Configuration items can be stored dynamically or be loaded from a
 * file. All configuration options are available throughout the game, to every
 * entity and every lib file, so take care of avoiding key collisions, for
 * example by prefixing your keys.
 */

int config_set(char *key, char *value);

char *config_get(char *key);

int load_config_from_file(const char *config_file);

void config_destroy();
