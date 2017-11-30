#include "event_loader.h"

#include "../lib/error_handling.h"
#include "../../lib/lineread.h"

#include <stdlib.h>


Event **load_events_from_file(FILE *fp, int num_resources){
  if (!fp){
    HE("invalid parameters - no file given", "load_events_from_file")
    return NULL;
  }
  if (num_resources < 1){
    HE("invalid parameters - non-positive number of resources", "load_events_from_file")
    return NULL;
  }

  int num_events = 0;
  char *buff = fgetll(fp);
  if (!buff){
    HE("cannot determine the number of events to load", "load_events_from_file")
    return NULL;
  }
  num_events = atoi(buff);free(buff);

  int resource_list_len = 0;
  buff = fgetll(fp);
  if (!buff) {
      HE("cannot determine length of resource multiplier lists", "load_events_from_file");
      return NULL;
  }

  resource_list_len = atoi(buff); free(buff);
  if (resource_list_len != num_resources) {
      HE("refusing to load event db file with a diferent number of resource multiplier than expected", "load_events_from_file");
      return NULL;
  }

  Event **events = calloc (num_events + 1, sizeof(Event *));
  if (!events){
    HE("cannot load events, out of memory", "load_events_from_file")
    return NULL;
  }

  for (int i = 0; i < num_events; i++){
    buff = fgetll(fp);
    int id = atoi(buff); free(buff);
    buff = fgetll(fp);
    int num_turns = atoi(buff); free(buff);
    char *name = fgetll(fp);
    if (!name){
      HE("could not retrieve name for an event", "load_events_from_file")
      event_list_destroy(events); return NULL;
    }

    float *mult = calloc(MAX_RESOURCES, sizeof(float));
    if(!mult){
      HE("could not allocate memory for resources multipliers", "load_events_from_file");
      free(name); event_list_destroy(events);return NULL;
    }
    for (int j = 0; j < num_resources; j++){
      buff = fgetll(fp);
      if(!buff){
        HE("error reading multipliers from event", "load_events_from_file");
        free(name);free(mult);event_list_destroy(events);
      }
      mult[j] = atof(buff);free(buff);
    }
    events[i] = event_new (name, mult, id, num_turns);
    free(name);free(mult);
    if(!events[i]){
      HE("could not create event", "load_events_from_file")
      event_list_destroy(events);return(NULL);
    }
  }
  return events;
}

void event_list_destroy(Event **list)
{
    if (!list) return;

    for (int i = 0; list[i]; event_destroy(list[i++]));
    free(list);
}
