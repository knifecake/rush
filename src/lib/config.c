#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error_handling.h"
#include "../../lib/lineread.h"

#define MAX_DICT_SIZE 25

typedef struct {
    char **keys;
    char **values;
    int size;
} ConfigDict;

ConfigDict *cd;

ConfigDict *_config_dict_new()
{
    ConfigDict *cd = calloc(1, sizeof(ConfigDict));
    if (!cd) {
        HE("could not allocate config dictionary", "_config_dict_new");
        return NULL;
    }

    cd->keys = calloc(MAX_DICT_SIZE, sizeof(char *));
    if (!cd->keys) {
        HE("could not allocate key space", "_config_dict_new");
        free(cd); return NULL;
    }

    cd->values = calloc(MAX_DICT_SIZE, sizeof(char *));
    if (!cd->values) {
        HE("could not allocate value space", "_config_dict_new");
        free(cd->keys); free(cd);
        return NULL;
    }

    return cd;
}

int config_set(char *key, char *value)
{
    if (!key || !value) {
        HE("invalid parameters", "config_set");
        return UINT_ERROR;
    }

    // initialize a dictionary for config if there isn't one already
    if (!cd)
        cd = _config_dict_new();

    if (cd->size >= MAX_DICT_SIZE) {
        HE("no more config items fit into the config dictionary, increase MAX_DICT_SIZE", "config_set");
        return UINT_ERROR;
    }

    cd->keys[cd->size] = malloc(strlen(key) + 1);
    if (!cd->keys[cd->size]) {
        HE("out of memory", "config_set");
        return UINT_ERROR;
    }

    cd->values[cd->size] = malloc(strlen(value) + 1);
    if (!cd->values[cd->size]) {
        HE("out of memory", "config_set");
        return UINT_ERROR;
    }

    // copy k&v and increase dict size
    strcpy(cd->keys[cd->size], key);
    strcpy(cd->values[cd->size++], value);

    return cd->size;
}

void _config_dict_destroy(ConfigDict *cd)
{
    if (!cd)
        return;

    for (int i = 0; i < cd->size; i++) {
        free(cd->keys[i]);
        free(cd->values[i]);
    }

    free(cd->keys);
    free(cd->values);
    free(cd);
}

char *config_get(char *key)
{
    if (!key) {
        HE("invalid parameters – key must not be NULL", "config_get");
        return NULL;
    }

    if (!cd)
        return NULL;

    for (int i = 0; i < cd->size; i++) {
        if (strcmp(key, cd->keys[i]) == 0) {
            return cd->values[i];
        }
    }

    return NULL;
}

int load_config_from_file(const char *config_file)
{
    if (!config_file) {
        HE("invalid parameters", "load_config_from_file");
        return UINT_ERROR;
    }

    FILE *f = fopen(config_file, "r");
    if (!f) {
        HE("could not open config file", "load_config_from_file");
        return UINT_ERROR;
    }
    char *key;
    char *value;
    while ((key = fgetll(f)))
    {
        if (!(value = fgetll(f))) {
            HE("invalid config file. found a key without an associated value", "load_config_from_file");
            free(key);
            return UINT_ERROR;
        }

        config_set(key, value);
        free(key);
        free(value);
    }

    fclose(f);

    return cd->size;
}

void config_destroy()
{
    if (cd)
        _config_dict_destroy(cd);
}
