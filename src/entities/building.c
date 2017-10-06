#include "building.h"

struct _Building {
  size_t id;   /* Building id*/
  int level;  /* Level building */
  int unlocking_level;  /* Player level requiered to build it*/
  int health; /* Health points of the building */
  int cost;   /*Cost of building or leveling it up*/
  int tile_id;  /*Id where it is built*/
  int base_resources; /* Quantity of resources returned each time */
};

Building *building_new (size_t id){
  Building *bp;
  if (!bp = calloc(1, sizeof(Building))) return NULL;
  bp -> id = id;
  /*
   * This piece of code should be substituted with the initial values depending
   * on the id of this building. Probably a switch statement.
   */
  return bp;
}

void building_destroy (Building *bp){
  if (bp) free(bp);
}

Building *building_level_up (Building *bp){

}

int building_collect_resources (Building *bp){

}

Building *building_place (Building *bp, int tile_id){

}

Building *building_edit_health (Building* bp, int increment){

}

size_t building_get_id (Building *bp){

}

int building_get_level (Building *bp){

}

int building_get_unlocking_level (Building *bp){

}

int building_get_health (Building *bp){

}

int building_get_cost (Building *bp){

}

int building_get_tile_id (Building *bp){

}

int building_get_base_resources (Building *bp){

}
