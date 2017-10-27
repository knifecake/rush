#include "event.h"

#include "../lib/error_handling.h"

#include <string.h>
#include <stdlib.h>

struct _Event{
  int id;
  int num_turns;
  char *name;
  float mult[MAX_RESOURCES];
};

Event *event_new (char *name, float *mult, int id, int num_turns){
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
    HE("incorrect values for id or num_turns", "event_new")
    return NULL;
  }

  Event *e;

  e = calloc(1, sizeof(Event));
  e->id = id;
  e->num_turns = num_turns;
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

void event_print(FILE *s, Event *e)
{
    if (!s || !e) {
        HE("invalid parameters", "event_print")
        return;
    }

    fprintf(s, "Event %s, id = %d, num_turn = %d\n", e->name, e->id, e->num_turns);
}
