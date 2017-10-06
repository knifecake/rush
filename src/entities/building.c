#include "building.h"

struct _building {
  size_t id;   /* Building id*/
  size_t level;  /* Level building */
  size_t unlocking_level;  /* Player level requiered to build it*/
  size_t health; /* Health points of the building */
  size_t cost;   /*Cost of building or leveling it up*/
  size_t tile_id;  /*Id where it is built*/
  size_t base_resources; /* Quantity of resources returned each time */

  /* We're using size_t so whe don't have to check the value is not negative */
};

Building *building_new (size_t id){
  Building *bp;
  if (!bp = calloc(1, sizeof(Building))){
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
  if (!bp){
    fprintf(stderr, "building_destroy: Error! Pointer is NULL\n");
    return;
  }
  free (bp);
}

Building *building_level_up (Building *bp){
  if(!bp){
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

Building *building_place (Building *bp, size_t tile_id){
  if(!bp){
    fprintf(stderr, "building_place: Error! Pointer is NULL\n");
    return NULL;
  }
  if (tile_id > MAX_TILE){
    fprintf(stderr, "building_place: Error! tile_id exceeds from limits\n");
    return NULL;
  }
  bp->tile_id = tile_id;
  return bp;
}

Building *building_edit_health (Building* bp, int increment){
  if(!bp){
    fprintf(stderr, "building_edit_health: Error! Pointer is NULL\n");
    return NULL;
  }
  if (increment < 0 && (-1*increment) => bp -> health){
    bp -> health = 0;
  }else{
    bp -> health += increment;
  }
  return bp;
}

size_t building_get_id (Building *bp){

}

size_t building_get_level (Building *bp){

}

size_t building_get_unlocking_level (Building *bp){

}

size_t building_get_health (Building *bp){

}

size_t building_get_cost (Building *bp){

}

size_t building_get_tile_id (Building *bp){

}

size_t building_get_base_resources (Building *bp){

}
