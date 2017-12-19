#include <string.h>

#include "minitest.h"

#include "../src/entities/event.h"
#include "../src/asset_loaders/event_loader.h"
#include "../src/lib/error_handling.h"
#include "../lib/lineread.h"

/*
 * A test specification for Event loader.
 */
#define EVENTS_DB "test/assets/example_event_loader.txt"

int main(void) {
  FILE *ef = fopen(EVENTS_DB, "r");
  int num_resources = 2;
  float* multipliers;
  if(!ef){
    assert("could not open events_db file", false);
    return failed_tests();
  }

  Event **events;
  assert("cannot create events from null file", !load_events_from_file(NULL, num_resources));
  assert("cannot create events with non-positive resources number", !load_events_from_file(ef, -1));

  events = load_events_from_file(ef, num_resources);
  fclose(ef);
  assert("can create events from file", events);
  if (!events) {
      return failed_tests();
  }

  assert("id read correctly", event_get_id(events[0]) == 1);

  assert("num_turns read correctly", event_get_num_turns(events[0]) == 3);

  assert("name read correctly", 0 == strcmp(event_get_name(events[0]),"earthquake"));

  multipliers = event_get_mult(events[0]);
  assert("first multiplier read correctly", 0.2f == multipliers[0]);
  assert("second multiplier read correctly", 0.41f == multipliers[1]);

  event_list_destroy(events);
  return failed_tests();
}
