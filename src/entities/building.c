#include <string.h>

#include "building.h"

#include "../lib/error_handling.h"

#define MAX_DESCRIPTION 100
#define MAX_SPRITE_NAME 32

struct _Building {
  int id;   /* Building id*/
  int level;  /* Level building */
  int unlocking_level;  /* Player level requiered to build it*/
  int health; /* Health points of the building */
  int cost[MAX_RESOURCES];   /*Cost of building or leveling it up*/
  int base_resources[MAX_RESOURCES]; /* Quantity of resources returned each time */
  char sprite[MAX_SPRITE_NAME + 1];
  char description[MAX_DESCRIPTION + 1];
};

Building *building_new (int id, int level, int unlocking_level, int health,
int * cost, int * base_resources, const char *sprite, const char *description){
  if(!sprite){
    HE("cannot create building, missing sprite name", "building_new")
    return NULL;
  }
  if (strlen(sprite) > MAX_SPRITE_NAME){
    HE("cannot create building, sprite name too long", "building_new")
    return NULL;
  }
  if (!description || strlen(description) > MAX_DESCRIPTION) {
      HE("description missing or too long", "building_new");
      return NULL;
  }
  Building *bp = oopsalloc(1, sizeof(Building), "building_new");
  strcpy(bp->sprite, sprite);
  strcpy(bp->description, description);
  bp -> id = id;
  bp -> level = level;
  bp -> unlocking_level = unlocking_level;
  bp -> health = health;
  for (int i = 0; i < MAX_RESOURCES; i++){
    bp->base_resources[i] = base_resources[i];
    bp -> cost[i] = cost[i];
  }
  /*
   * This piece of code should be substituted with the initial values depending
   * on the id of this building. Probably a switch statement.
   */
  return bp;
}

Building *building_copy(const Building *original)
{
    if (!original) {
        HE("invalid arguments", "building_copy");
        return NULL;
    }

    return building_new(
            original->id,
            original->level,
            original->unlocking_level,
            original->health,
            (int *)original->cost,
            (int *)original->base_resources,
            original->sprite,
            original->description);
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
    return UINT_ERROR;
  }
  return bp -> id;
}

char *building_get_name(Building *bp)
{
    if (!bp) {
        HE("invalid parameters", "building_get_name");
        return NULL;
    }

    return bp->sprite;
}

int building_get_level (Building *bp){
  if(!bp){
    HE("pointer is NULL", "building_get_level");
    return UINT_ERROR;
  }
  return bp -> level;
}

int building_get_unlocking_level (Building *bp){
  if(!bp){
    HE("pointer is NULL", "building_get_unlocking_level");
    return UINT_ERROR;
  }
  return bp -> unlocking_level;
}

int building_get_health (Building *bp){
  if(!bp){
    HE("pointer is NULL", "building_get_health");
    return UINT_ERROR;
  }
  return bp -> health;
}

int building_get_cost (Building *bp, const int resource_id){
  if(!bp){
    HE("pointer is NULL", "building_get_cost");
    return UINT_ERROR;
  }
  if (resource_id >= MAX_RESOURCES)
      return UINT_ERROR;

  return bp->cost[resource_id];
}

int building_get_base_resources (Building *bp, const int resource_id){
  if(!bp){
    HE("pointer is NULL\n", "building_get_base_resources");
    return -1;
  }
  return bp -> base_resources[resource_id];
}

char *building_get_sprite(Building *bp)
{
    if (!bp) {
        HE("invalid arguments", "building_get_sprite");
        return NULL;
    }

    return bp->sprite;
}

char *building_get_description(Building *bp)
{
    if (!bp) {
        HE("invalid arguments", "building_get_sprite");
        return NULL;
    }

    return bp->description;
}

Building *building_set_health(Building *b, int health)
{
    if(!b || health < 0) {
      HE("invalid arguments", "building_set_health");
      return NULL;
    }

    b->health = health;
    return b;
}

int building_is_townhall(Building *bp)
{
    if (!bp) {
        HE("invalid arguments", "building_is_townhall");
        return 0;
    }

    // TODO: change this to be loaded from config
    if (bp->id >= 101 && bp->id <= 105) {
        return 1;
    }

    return 0;
}

int building_is_upgrade(Building *b1, Building *b2)
{
    if (!b1 || !b2)
        return 0;

    // buildings must share the first digits of the id in addition to
    // having consecutive ids
    return b1->level / 10 == b2->level / 10 && b1->id == b2->id + 1;
}

bool building_is_market(Building *b){
  if(!b) return false;
  if(b->id == 151|| b->id == 152) return true;
  return false;
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

    fprintf(f, "\n - resource no.:                 ");
    for (int i = 0; i < MAX_RESOURCES; fprintf(f, "%8d ", i++));

    fprintf(f, "\n - resources needed to build: ");
    for (int i = 0; i < MAX_RESOURCES; fprintf(f, "%8d ", bp->cost[i++]));
	fprintf(f, "\n");

    fprintf(f, "\n - resources returned each time: ");
    for (int i = 0; i < MAX_RESOURCES; fprintf(f, "%8d ", bp->base_resources[i++]));
	fprintf(f, "\n");
}
