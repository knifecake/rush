#include "building.h"

struct _Building {
  int id;   /* Building id*/
  int level;  /* Level building */
  int unlocking_level;  /* Player level requiered to build it*/
  int health; /* Health points of the building */
  int cost;   /*Cost of building or leveling it up*/
  int base_resources[MAX_RESOURCE]; /* Quantity of resources returned each time */

  /* We're using int so whe don't have to check the value is not negative */
};

Building *building_new (int id){
  Building *bp;
  if (!(bp = calloc(1, sizeof(Building)))) {
    fprintf(stderr, "building_new: Error! Memory not allocated\n");
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
    fprintf(stderr, "building_destroy: Error! Pointer is NULL\n");
    return;
  }
  free (bp);
}

Building *building_level_up (Building *bp){
  if (!bp) {
    fprintf(stderr, "building_level_up: Error! Pointer is NULL\n");
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
    fprintf(stderr, "building_edit_health: Error! Pointer is NULL\n");
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
    fprintf(stderr, "building_get_id: Error! Pointer is NULL\n");
    return -1;
  }
  return bp -> id;
}

int building_get_level (Building *bp){
  if(!bp){
    fprintf(stderr, "building_get_level: Error! Pointer is NULL\n");
    return -1;
  }
  return bp -> level;
}

int building_get_unlocking_level (Building *bp){
  if(!bp){
    fprintf(stderr, "building_get_unlocking_level: Error! Pointer is NULL\n");
    return -1;
  }
  return bp -> unlocking_level;
}

int building_get_health (Building *bp){
  if(!bp){
    fprintf(stderr, "building_get_health: Error! Pointer is NULL\n");
    return -1;
  }
  return bp -> health;
}

int building_get_cost (Building *bp){
  if(!bp){
    fprintf(stderr, "building_get_cost: Error! Pointer is NULL\n");
    return -1;
  }
  return bp -> cost;
}

int building_get_base_resource (Building *bp, const int resource_id){
  if(!bp){
    fprintf(stderr, "building_get_base_resources: Error! Pointer is NULL\n");
    return -1;
  }
  return bp -> base_resources[resource_id];
}
