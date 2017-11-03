#include "building_loader.h"

#include "../lib/error_handling.h"
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

        buff = fgetll(f);
        int unlocking_level = atoi(buff); free(buff);

        buff = fgetll(f);
        int health = atoi(buff); free(buff);

        buff = fgetll(f);
        int cost = atoi(buff); free(buff);

        int base_resources[MAX_RESOURCES];
        for(int j = 0; j < num_resources; j++){
            buff = fgetll(f);
            base_resources[j] = atoi(buff); free(buff);
        }

        buildings[i] = building_new(id, level, unlocking_level, health, cost,
          base_resources);
    }

    return buildings;
}

void building_list_destroy(Building **l)
{
    if (!l) return;
    for (int i = 0; l[i++]; building_destroy(l[i]));
    free(l);
}
