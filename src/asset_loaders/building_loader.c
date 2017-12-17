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
        free(buff); return NULL;
    }
    num_buildings = atoi(buff); free(buff);

    int resource_list_len = 0;
    buff = fgetll(f);
    if (!buff) {
        HE("cannot determine the length of resource multiplier lists", "load_buildings_from_file");
        free(buff); return NULL;
    }

    resource_list_len = atoi(buff); free(buff);
    if (resource_list_len != num_resources) {
        HE("refusing to load a diffrenent number of resource multipliers than resources were loaded", "load_buildings_from_file");
        return NULL;
    }

    //we'll leave the last pointer set to null, to signal the end of the list
    Building **buildings = oopsalloc(num_buildings + 1, sizeof(Building *), "load_buildings_from_file");

    for (int i = 0; i < num_buildings; i++){
        buff = fgetll(f);
        int id = atoi(buff); free(buff);

        char *sprite = fgetll(f);

        buff = fgetll(f);
        int level = atoi(buff); free(buff);

        buff = fgetll(f);
        int unlocking_level = atoi(buff); free(buff);

        buff = fgetll(f);
        int health = atoi(buff); free(buff);

        buff = fgetll(f);
        int cost = atoi(buff); free(buff);

        int base_resources[MAX_RESOURCES] = { 0 };
        for(int j = 0; j < num_resources; j++){
            buff = fgetll(f);
            base_resources[j] = atoi(buff); free(buff);
        }

        buildings[i] = building_new(id, level, unlocking_level, health, cost,
          base_resources, sprite);

        free(sprite);
    }
    return buildings;
}

void building_list_destroy(Building **l)
{
    if (!l) return;
    for (int i = 0; l[i++]; building_destroy(l[i]));
    free(l);
}
