#include "resource_loader.h"

#include "../error_handling.h"
#include "../../lib/lineread.h"

#include <stdlib.h>


int load_resources_from_file(FILE *f, Resource **resources)
{
    if (!f || !resources) {
        HE("invalid parameters", "load_resources_from_file")
        return PINT_ERROR;
    }

    int num_resources = 0;
    num_resources = atoi(fgetll(f));

    // never load more than MAX_RESOURCES resources
    num_resources = (num_resources > MAX_RESOURCES) ? MAX_RESOURCES : num_resources;

    for (int i = 0; i < num_resources; i++) {
        char *buff = fgetll(f);
        int id = atoi(buff); free(buff);
        char *name = fgetll(f);

        if (!name) {
            HE("could not retrieve name for a resource", "load_resources_from_file")
            resource_list_destroy(resources, i - 1); return PINT_ERROR;
        }

        resources[i] = resource_new(id, name);
        if (!resources[i]) {
            HE("could not create resource", "load_resources_from_file")
            free(name); resource_list_destroy(resources, i - 1); return PINT_ERROR;
        }

        free(name);
    }

    return num_resources;
}


void resource_list_destroy(Resource **list, int len)
{
    if (!list) return;

    for (int i = 0; i < len; resource_destroy(list[i++]));
    free(list);
}
