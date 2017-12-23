#ifndef __DICT_H__
#define __DICT_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct _Dict Dict;

typedef int (*key_cmp_func)(void *k1, void *k2);
typedef int (*value_cmp_func)(void *v1, void *v2);
typedef void (*value_free_func)(void *v);

/*
 * Creates a genereal diictionary from a file, allocating the functions to compare the
 * values and keys given. Returns NULL on error.
 */
Dict *dict_new(int, key_cmp_func, value_cmp_func, value_free_func);

/*
 *  Returns the value associated with the given key. NULL on Error.
 */
void *dict_get(Dict *, void *);

/*
 * Stores a key-value pair in the config dictionary. Returns UINT_ERROR on
 * error. If the given key was already stored, it's value is overriden by the
 * new one. Returns UINT_ERROR on error;
 */
int dict_set(Dict *, void *key, void *value);

/*
 * Frees all resources allocated by the Dict.
 */
void dict_destroy(Dict *);

/* Compare functions */
int cmp_string(void *, void *);
int cmp_sprite(void *, void *);
int cmp_int(void *, void *);
/* Value free functions */
void free_sprite(void *);
void free_int(void *);

#endif
