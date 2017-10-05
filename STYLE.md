# Style guide

## Naming conventions

  * Types are titleized, that is, they have the first letter of each word capitalized, without any underscores. Good: `Tile, WorldMap`, bad: `TILE, tile, worldMap, world_map, World_Map`.
  * Variables are declared in `snake_case`, that is, all lower case letters with words separated by hyphens. Good: `enemies, available_resources`, bad: `Enemies, availableResources`.
  * Entity methods (functions) are prefixed with the lowercase version of the Type of that entity and follow the same convention as variables, e.g. `building_destroy`, not `buildingDestroy`, or `building_get_enemies` rather than `building_getEnemies`.
  * Filenames follow the same convetion as variable names, except when other conventions apply (i.e. `README.md`, etc).
  * Constants are declared in `ALL_CAPS`, with underscores as separators.
  * Function prototypes do not include argument names, only the types. Good: `Building *building_destroy(Building *)`.
  * Names for initializers and destroy methods in entities follow the `Entity *entity_new(), void entity_destroy(Entity *)` pattern.


## Placement of keywords, delimiters and operators

  * The pointer `*` should be placed next to the variable or function name, not next to the type. Good: `int *list, Building *building_new()`, bad: `int* list, Building* building_new()`.
  * Indent your code with `2 spaces`. Do not leave trailing whitespace at the end of any line. Use unix-style line endings (`\n`, not `\r\n`). Encode your files in `utf8`.
  * Curly braces are placed on the same line in `for, while, do-while` and `if-elsif-else` constructs, but on a different line for functions.

## Further considerations

  * Comparisons with `NULL` should be avoided: use `!something` instead of `something == NULL` to check for a `NULL` value.
  * Include `<stdbool.h>` for boolean types. They are declared like this: `bool something = true`. `true` and `false` keywords are lowercase, as all other C keywords.
  * Write descriptive commit messages. Do not end them with a period
  * Be succinct.
