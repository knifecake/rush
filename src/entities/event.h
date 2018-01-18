/*
 * PPROG Game – Morzilla Firefox 2018
 *
 * Authors: Miguel Baquedano, Sergio Cordero, Elias Hernandis
 *          and Rafael Sánchez.
 *
 * Lead author: <replace me>
 */


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
 * Maximum length of the event name.
 */
#define MAX_EVENT_NAME 32

typedef struct _Event Event;

/*
 * Creates an event given the name, resource_multipliers, id and number of
 * turns on an envent. Name is copied on creation and freed on deletion.
 *
 * Returns NULL on error. Num_Turns NEED to be >= 1.
 */
Event *event_new(char*, float*, int, int, int);


/*
 * Frees memory given an Event. This will be managed by the tile when the event
 * comes to an end. RETURNS NULL. Usage MUST BE "e = event_destroy(e);"
 */
void event_destroy(Event *);

/*
 * Creates a copy of event src.
 * Returns the copy on success.
 * Returns NULL on error.
 */
Event *event_copy (Event *src);

/*
 * This function decrease in 1 the number of turns remaining each time.
 * If n_turns = 0, it will exit successfully.
 */
Event *event_next_turn(Event *);
/*
 * Getters.
 */
int event_get_id (Event *);
char *event_get_name(Event *);
float *event_get_mult(Event *);
int event_get_num_turns(Event *);
int event_get_damage(Event *);

Event* event_set_remaining_turns(Event *, int);
/*
 * Prints the given resource, for debugging purposes.
 */
void event_print(FILE *, Event *);

#endif
