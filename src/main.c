#include <stdio.h>

#include "../lib/lineread.h"

#include "error_handling.h"

#include "entities/resource.h"

#include "asset_loaders/resource_loader.h"


/*
 * Provisional constant.
 *
 * Describes where are we loading our resources from relative to the directory
 * from where the executable file will be called. See Makefile or README.md for
 * information on where this file is.
 */
#define RESOURCES_DB "assets/resources.txt"


int main(void) {
    FILE *rf = fopen(RESOURCES_DB, "r");
    if (!rf) {
        handle_error("could not open resources db file", "main", __FILE__, __LINE__);
        return PINT_ERROR;
    }

    Resource **resources = malloc(MAX_RESOURCES * sizeof(Resource *));
    int num_resources = load_resources_from_file(rf, resources);
    if (num_resources == 0) {
        handle_error("could not load resources", "main", __FILE__, __LINE__);
        return PINT_ERROR;
    }

    fclose(rf);

    for (int i = 0; i < num_resources; resource_print(stdout, resources[i++]));

    resource_list_destroy(resources, num_resources);

    return 0;
}
