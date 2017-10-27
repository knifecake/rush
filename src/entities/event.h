#ifndef __EVENT_H__
#define __EVENT_H__

/*
 * event.h
 *
 * An event that randomly occurs during a run of the game.
 * It consists in:
 *  id: must be 0 or greater.
 *  name: cannot be null, <32 chars.
 *  effect_on_resources: resource multipliers, cannot be null.
 *  num_turns: number of turns that lasts the event. This will be decreased
 *  by the tile. 1 or greater.
 *
 * Actions:
 * Create new event.
 * Destroy an event.
 *
 */


#include <stdio.h>
#include <stdlib.h>

#include "resource.h"

/*
 * The maximun number of Events the game currently supports.
 */
#define MAX_EVENTS 5


/*
 * Maximum length of the event name.
 */
#define MAX_EVENT_NAME 32

typedef struct _Event Event;

/*
 * Creates an event given the name, resource_multipliers, id and number of
 * turns on an envent. Name is copied on creation and freed on deletion.
 *
 * Returns NULL on error.
 */
Event *event_new(char*, float*, int, int);


/*
 * Frees memory given an Event. This will be managed by the tile when the event
 * comes to an end.
 */
void event_destroy(Event *);

/*
 * Getters.
 */
int event_get_id (Event *);
char *event_get_name(Event *);
float *event_get_mult(Event *);
int event_get_num_turns(Event *);

/*
 * Prints the given resource, for debugging purposes.
 */
void event_print(FILE *, Event *);

#endif
