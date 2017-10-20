#include "building.h"

#include "../error_handling.h"

struct _Building {
  int id;   /* Building id*/
  int level;  /* Level building */
  int unlocking_level;  /* Player level requiered to build it*/
  int health; /* Health points of the building */
  int cost;   /*Cost of building or leveling it up*/
  int base_resources[MAX_RESOURCES]; /* Quantity of resources returned each time */
};

Building *building_new (int id){
  Building *bp;
  if (!(bp = calloc(1, sizeof(Building)))) {
    HE("memory not allocated", "building_new");
    return NULL;
  }
  bp -> id = id;
  /*
   * This piece of code should be substituted with the initial values depending
   * on the id of this building. Probably a switch statement.
   */
  return bp;
}

void building_destroy (Building *bp){
  if (!bp) {
    HE("pointer is NULL", "building_destroy");
    return;
  }
  free (bp);
}

Building *building_level_up (Building *bp){
  if (!bp) {
    HE("building_level_up: Error! Pointer is NULL", "building_level_up");
    return NULL;
  }
  bp->level++;
  /*
   * This piece of code should be substituted with the increments of cost,
   * health and base_resources fields.
   */
   return bp;
}

Building *building_edit_health (Building* bp, int increment){
  if(!bp){
    HE("pointer is NULL", "building_edit_health");
    return NULL;
  }
  if (increment < 0 && (-1*increment) >= bp -> health){
    bp -> health = 0;
  }else{
    bp -> health += increment;
  }
  return bp;
}

int building_get_id (Building *bp){
  if(!bp){
    HE("Pointer is NULL", "building_get_id");
    return -1;
  }
  return bp -> id;
}

int building_get_level (Building *bp){
  if(!bp){
    HE("pointer is NULL", "building_get_level");
    return -1;
  }
  return bp -> level;
}

int building_get_unlocking_level (Building *bp){
  if(!bp){
    HE("pointer is NULL", "building_get_unlocking_level");
    return -1;
  }
  return bp -> unlocking_level;
}

int building_get_health (Building *bp){
  if(!bp){
    HE("pointer is NULL", "building_get_health");
    return -1;
  }
  return bp -> health;
}

int building_get_cost (Building *bp){
  if(!bp){
    HE("pointer is NULL", "building_get_cost");
    return -1;
  }
  return bp -> cost;
}

int building_get_base_resources (Building *bp, const int resource_id){
  if(!bp){
    HE("pointer is NULL\n", "building_get_base_resources");
    return -1;
  }
  return bp -> base_resources[resource_id];
}
