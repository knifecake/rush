# Style guide

## Naming conventions

  * Types are titleized, that is, they have the first letter of each word capitalized, without any underscores. Good: `Tile, WorldMap`, bad: `TILE, tile, worldMap, world_map, World_Map`.
  * Variables are declared in `snake_case`, that is, all lower case letters with words separated by hyphens. Good: `enemies, available_resources`, bad: `Enemies, availableResources`.
  * Entity methods (functions) are prefixed with the lowercase version of the Type of that entity and follow the same convention as variables, e.g. `building_destroy`, not `buildingDestroy`, or `building_get_enemies` rather than `building_getEnemies`.
  * Filenames follow the same convetion as variable names, except when other conventions apply (i.e. `README.md`, etc).
  * Constants are declared in `ALL_CAPS`, with underscores as separators.
  * Function prototypes do not include argument names, only the types. Good: `Building *building_destroy(Building *)`.
  * Names for initializers and destroy methods in entities follow the `Entity *entity_new(), void entity_destroy(Entity *)` pattern.
  * Maximum will be named by `MAX` and number with `NUM`


## Placement of keywords, delimiters and operators

  * The pointer `*` should be placed next to the variable or function name, not next to the type. Good: `int *list, Building *building_new()`, bad: `int* list, Building* building_new()`.
  * Indent your code with `2 spaces`. Do not leave trailing whitespace at the end of any line. Use unix-style line endings (`\n`, not `\r\n`). Encode your files in `utf8`.
  * Curly braces are placed on the same line as `for, while, do-while, if-elsif-else` constructs and `functions`.
  * Function arguments should be in the following order: files, function pointers, entities, other pointers, static variables.
  * Avoid declaring variables atop functions. They should be declared as close to their first use as posible. This is especially true for iterators.
  * The access to struct fields with -> will be done like `struct -> field` and not `struct->field`
  * Operators and if-clauses parenthesis will be delimited by spaces. Good: `a = b`. Bad: `a=b`
  * The usage of size_t type should be use inside iterators. The print - escape code for this type of data is %zu.

## Error handling

  * Error handling should print the error information in standard error.
    * Use the next function prototype: `fprintf( stderr, "<name_of_your_method>: error information \n" );`.
    * Example: In the function `building_new`, the function will be `fprintf( sterr, "building_new: Error! Memory not allocated \n");`
  * Alternatively, errors can be handle by `handle_error` declared in `src/error_handling.h`. Using this function is essentially equal to using the previous method but it can provide more comprehensive error descriptions more easily. It can be used in to ways:
    1. Analogously to the `fprintf(stderr, ...)` way: you may call `handle_error("error_message\n")` and it will be outputted to the appropriate log file or `stderr`. Take care to end your error message with a `\n`. Not doing so instructs the function to behave in the second way.
    2. In addition to the error message, you may pass the name of the function that raised the error, the filename where that function lives, and the line number where the call to handle_error was triggered. Although this may seem intimidating, it can be easily done with `handle_error`. Just call it like this: `handle_error("some error message", "draw_tile", __FILE__, __LINE__);`. In that way, the preprocessor will substitute for the appropriate file and line numbers. Refer to `handle_error.h` for further documentation.


## Further considerations

  * Comparisons with `NULL` should be avoided: use `!something` instead of `something == NULL` to check for a `NULL` value.
  * Include `<stdbool.h>` for boolean types. They are declared like this: `bool something = true`. `true` and `false` keywords are lowercase, as all other C keywords.
  * Write descriptive commit messages. Do not end them with a period
  * Thoroughly comment your code.
    * Place a comment before each function (before its prototype in the header file) describing what it does and how it's used.
    * Code inside a function should not need comments in every line. Descriptive variable names, clear control flow and keeping functions to the point should be enough to understand what a function does.
  * Be succinct.

## Example of a file header in comments
```
/*
 * tile.h
 *
 * A place is the basic unit of space. It has the following attributes:
 *  - tile_id: unique identifier for this cell
 *  - building_type: id of building type, -1 if no building
 *  - resource_multipliers: the amounts of each resource that can be obtained from this tile
 *  - remaining_resources: the amount of each resource remaining on this tile
 *  - visible: whether this tile is visible to the player
 *  - enemies: the number of enemies currently on this tile, -1 if conquered
 *  - sprite: tile image
 *
 * Actions:
 *  - build_a_building
 *  - draw_tile(x, y) draws the tile starting from (x, y)
 *
 */
```
