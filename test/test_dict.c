/*
 * PPROG Game – Morzilla Firefox 2018
 *
 * Authors: Miguel Baquedano, Sergio Cordero, Elias Hernandis
 *          and Rafael Sánchez.
 *
 * Lead author: <replace me>
 */


#include <string.h>

#include "minitest.h"

#include "../src/lib/dict.h"

int main() {
  Dict* d;

  assert("cannot create a dict with an invalid size", !dict_new(0, cmp_string, cmp_int, free_int));
  assert("can create a dict", d = dict_new(50, cmp_string, cmp_int, free_int));

  int *x;
  char *string;
  string = calloc(13, sizeof(char));
  strcpy(string, "gametest0001");
  x = calloc(1, sizeof(int));
  *x = 4;

  assert("can set a key-value pair", 1 == dict_set(d, string, x));
  assert("can get a value given a key", 0 == cmp_int(x, dict_get(d, string)));

  int *y;
  y = calloc(1, sizeof(int));
  *y = 3;

  assert("can update a value given the same key", 1 == dict_set(d, string, y) && 0 == cmp_int(y, dict_get(d, string)));

  char *string2;
  string2 = calloc(13, sizeof(char));
  strcpy(string2, "gametest0002");

  assert("can add a new key-value pair", 2 == dict_set(d, string2, x));
  dict_destroy(d);
  return failed_tests();
}
