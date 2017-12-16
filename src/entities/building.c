#include "building.h"

#include "../lib/error_handling.h"

struct _Building {
  int id;   /* Building id*/
  int level;  /* Level building */
  int unlocking_level;  /* Player level requiered to build it*/
  int health; /* Health points of the building */
  int cost;   /*Cost of building or leveling it up*/
  int base_resources[MAX_RESOURCES]; /* Quantity of resources returned each time */
};

Building *building_new (int id, int level, int unlocking_level, int health,
int cost, int * base_resources){
  Building *bp = oopsalloc(1, sizeof(Building), "building_new");

  bp -> id = id;
  bp -> level = level;
  bp -> unlocking_level = unlocking_level;
  bp -> health = health;
  bp -> cost = cost;
  for (int i = 0; i < MAX_RESOURCES; i++){
    bp->base_resources[i] = base_resources[i];
  }
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

char *building_get_name(Building *bp)
{
    if (!bp) {
        HE("invalid parameters", "building_get_name");
        return NULL;
    }

    // TODO: change to return the name of building when we define it
    char *name = oopsalloc(100, sizeof(char), "building_get_name");
    sprintf(name, "Building #%d", bp->id);
    return name;
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

void building_print(FILE *f, Building *bp)
{
	if (!bp) {
		HE("invalid params", "building_print");
		return;
	}


	fprintf(f, "Building %d:", bp->id);

	fprintf(f, "\n - level: %d", bp->level);
	fprintf(f, "\n - unlocking level: %d", bp->unlocking_level);
	fprintf(f, "\n - health: %d", bp->health);
	fprintf(f, "\n - cost: %d", bp->cost);


    fprintf(f, "\n - resource no.:                 ");
    for (int i = 0; i < MAX_RESOURCES; fprintf(f, "%8d ", i++));

    fprintf(f, "\n - resources returned each time: ");
    for (int i = 0; i < MAX_RESOURCES; fprintf(f, "%8d ", bp->base_resources[i++]));
	fprintf(f, "\n");
}
