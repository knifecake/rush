#include "building_loader.h"

#include "../error_handling.h"
#include "../../lib/lineread.h"

#include <stdlib.h>


Building **load_buildings_from_file(FILE *f, int num_resources)
{
    if (!f) {
        HE("invalid parameters (no file given)", "load_buildings_from_file")
        return NULL;
    }

    int num_buildings = 0;
    char *buff = fgetll(f);
    if (!buff) {
        HE("cannot determine the number of buildings to load", "load_buildings_from_file")
        return NULL;
    }
    num_buildings = atoi(buff); free(buff);

    //we'll leave the last pointer set to null, to signal the end of the list
    Building **buildings = calloc(num_buildings + 1, sizeof(Building *));
    if(!buildings) {
        HE("cannot load buildings, out of memory", "load_buildings_from_file")
        return NULL;
    }

    for (int i = 0; i < num_buildings; i++){
        buff = fgetll(f);
        int id = atoi(buff); free(buff);

        buff = fgetll(f);
        int level = atoi(buff); free(buff);

        // TODO: unfinished
    }

    return buildings;
}
