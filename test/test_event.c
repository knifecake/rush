#include "minitest.h"
#include <string.h>
#include "../src/entities/event.h"

/*
 * A test specification for Event.
 */

int main(void) {
  int turns;
  float *mult;
  mult = calloc(MAX_RESOURCES, sizeof(float));
  for (size_t i = 0; i < MAX_RESOURCES-1; i++) {
    mult[i]= i * 2.3;
  }

  assert("cannot create event with invalid name", !event_new("thisstringislongerthanthirtytwocharacters", mult, 1, 3));
  assert("cannot create event with invalid multipliers", !event_new("sample1", NULL, 1, 3));
  assert("cannot create event with invalid id", !event_new("sample1", mult, -1, 3));
  assert("cannot create event with invalid number of turns", !event_new("sample1", mult, 1, 0));
  assert("cannot set next turn with invalid event", !event_next_turn(NULL));

  Event *e = event_new("sample", mult, 1, 3);
  assert("can create event with valid parameters", e);

  turns = event_get_num_turns(e); /* turns == 3 */
  e = event_next_turn(e); /* ¿e->turns == 2? */
  assert("number of turns decrease successfully", turns == event_get_num_turns(e) + 1);

  Event *ecopy=NULL;
  assert("cannot copy an event from NULL", !event_copy(ecopy, NULL));
  ecopy = event_copy(ecopy, e);
  assert("can copy an event with valid parameters", ecopy);

  assert("number of turns copied successfully", event_get_num_turns(e) == event_get_num_turns(ecopy));
  assert("id copied successfully", event_get_id(e) == event_get_id(ecopy));
  assert("name copied successfully", 0 == strcmp(event_get_name(e), event_get_name(ecopy)));

  ecopy = event_destroy(ecopy);

  e = event_next_turn(e);/* e->turns == 1 */
  e = event_next_turn(e);/* e->turns == 0 */
  e = event_next_turn(e);/* ¿e->turns == 0? */
  assert("number of turns stays as 0 when next_turn happens", 0 == event_get_num_turns(e));

  e = event_destroy(e);
  assert("can destroy events successfully", !e && !ecopy);
  assert("--FINISH TEST EVENT--", true);
  free(mult);
  return failed_tests();
}
