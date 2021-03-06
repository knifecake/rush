/*
 * PPROG Game – Morzilla Firefox 2018
 *
 * Authors: Miguel Baquedano, Sergio Cordero, Elias Hernandis
 *          and Rafael Sánchez.
 *
 * Lead author: <replace me>
 */


#include "event.h"

#include "../lib/error_handling.h"

#include <string.h>
#include <stdlib.h>

struct _Event{
  int id;
  int num_turns;
  int damage;
  char name[MAX_EVENT_NAME+1];
  float mult[MAX_RESOURCES];
};

Event *event_new (char *name, float *mult, int id, int num_turns, int damage){
  if(!name){
    HE("missing event name", "event_new")
    return NULL;
  }
  if (strlen(name) > MAX_EVENT_NAME) {
      HE("cannot create event, name too long", "event_new")
      return NULL;
  }
  if(!mult){
    HE("missing event\'s resouce multipliers", "event_new")
    return NULL;
  }
  if(id<0 || num_turns < 1){
      printf("id = %d, turns = %d, damage = %d", id, num_turns, damage);
    HE("incorrect values for id, num_turns or damage", "event_new")
    return NULL;
  }

  Event *e = oopsalloc(1, sizeof(Event), "event_new");
  e->id = id;
  e->num_turns = num_turns;
  e->damage = damage;
  for (size_t i = 0; i < MAX_RESOURCES-1; i++) {
    e->mult[i] = mult[i];
  }
  strcpy(e->name, name);
  return e;
}

void event_destroy(Event *e)
{
    free(e);
}

Event *event_copy(Event *src){
  if(!src){
    HE("invalid source event", "event_copy")
    return NULL;
  }
  Event *dest;
  dest = event_new(src->name, src->mult, src->id, src->num_turns, src->damage);
  if (!dest){
    HE("creating dest event was not possible", "event_copy")
    return NULL;
  }
  return dest;
}

Event *event_next_turn(Event *e){
  if(!e){
    HE("null event", "event_next_turn");
    return NULL;
  }
  if(e->num_turns == 0){
    return e;
  }
  e->num_turns--;
  return e;
}

int event_get_id (Event *e){
  if(!e){
    HE("null event", "event_get_id")
    return -1;
  }
  return e->id;
}
char *event_get_name(Event *e){
  if(!e){
    HE("null event", "event_get_name")
    return NULL;
  }
  return e->name;
}
float *event_get_mult(Event *e){
  if(!e){
    HE("null event", "event_get_mult")
    return NULL;
  }
  return e->mult;
}
int event_get_num_turns(Event *e){
  if(!e){
    HE("null event", "event_get_num_turns")
    return -1;
  }
  return e->num_turns;
}
int event_get_damage(Event *e){
  if(!e){
    HE("null event", "event_get_damage")
    return -1;
  }
  return e->damage;
}

Event* event_set_remaining_turns(Event *e, int rem_turns){
  if(!e || rem_turns < 0){
    HE("invalid arguments", "event_set_remaining_turns")
    return NULL;
  }
  e->num_turns = rem_turns;
  return e;
}

void event_print(FILE *s, Event *e)
{
    if (!s || !e) {
        HE("invalid parameters", "event_print")
        return;
    }

    fprintf(s, "Event %s, id = %d, num_turn = %d\n", e->name, e->id, e->num_turns);
}
