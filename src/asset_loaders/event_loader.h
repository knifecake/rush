/*
 * PPROG Game – Morzilla Firefox 2018
 *
 * Authors: Miguel Baquedano, Sergio Cordero, Elias Hernandis
 *          and Rafael Sánchez.
 *
 * Lead author: <replace me>
 */


/*
 * Event loader.
 *
 * Contains functions to read a list of events from a file.
 */

#include "../entities/event.h"

/*
 * Populates a list of pointers to events with events loaded from a file.
 * Returns the number of events loaded. This function will never load more than
 * MAX_EVENTS even if the database specifies there are more.
 * Thus, you can be confident that if you pass a buffer of MAX_EVENTS length to
 * this function, it will be able to open any file.
 *
 * The event database file must conform to this format:
 *
 * 1          # (int) id

 * 2          # (int) number of turns
 * tsunami    # (char *) name

 * 0.26       # (float) resource 0 multiplier
 * 0.31       # (float) resource 1 multiplier
 * ...
 * 0.23       # (float) resource n multiplier
 *
 * See the example event database provided in assets/events.txt for additional
 * details on the file format.
 */
Event **load_events_from_file (FILE *, int);

/*
 * Frees a list of pointers to events. Assumes list is len events long.
 *
 * TODO: consider this is a utility function. Maybe it should be somewhere else.
 */
 void event_list_destroy (Event **);
