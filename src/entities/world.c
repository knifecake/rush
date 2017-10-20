
#include <stdio.h>
#include <stdlib.h>

#include "world.h"
#include "../error_handling.h"


struct _World{
  Tile **map;
  Building **b;
};

World *world_new(Tile **map, Building **b){
  World *w=NULL;

  if(!t || !b){
    HE("invalid parameters","world_new");
    return NULL;
  }

  w=(World *)malloc(sizeof(wolrd));
  if(!w){
    HE("malloc error","world_new");
    return NULL;
  }

  return w;
}

void world_destroy(World *w){
  if(!w){
    HE("invalid parameters", "world_destroy");
    return;
  }

  free(w);
}

World *world_player_turn(World *w, Player *p){
  if(!w || !p){
    HE("invalid parameters", "world_player_turn");
    return NULL;
  }
  /*



  */
  return w;
}

World *world_ai_turn(World *w){
  if(!w){
    HE("invalid parameters", "world_player_turn");
    return NULL;
  }
  /*



  */
  return w;
}
