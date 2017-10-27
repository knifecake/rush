#include "minitest.h"

#include "../src/entities/event.h"

/*
 * A test specification for Event.
 */

int main(void) {
  float *mult;
  mult = calloc(MAX_RESOURCES, sizeof(float));
  for (size_t i = 0; i < MAX_RESOURCES-1; i++) {
    mult[i]= i * 2.3;
  }

  assert("cannot create event with invalid name", !event_new("thisstringislongerthanthirtytwocharacters", mult, 1, 3));
  assert("cannot create event with invalid multipliers", !event_new("sample1", NULL, 1, 3));
  assert("cannot create event with invalid id", !event_new("sample1", mult, -1, 3));
  assert("cannot create event with invalid number of turns", !event_new("sample1", mult, 1, 0));

  Event *e = event_new("sample", mult, 1, 3);
  assert("can create event with valid parameters", e);

  event_destroy(e);
  assert("can destroy event successfully", true);

  return 0;
}
