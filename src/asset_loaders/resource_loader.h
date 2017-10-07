/*
 * Resource loader.
 *
 * Contains functions to read a list of resources from a file.
 */


#include "../entities/resource.h"

/*
 *  Populates a list of pointers to resources with resources loaded from a
 *  file. Returns the number of resources loaded. This function will never load
 *  more than MAX_RESOURCES even if the database specifies there are more.
 *  Thus, you can be confident that if you pass a buffer of MAX_RESOURCES
 *  length to this function, it will be able to open any file.
 *
 * The resource database file must conform to this format:
 *
 *      2           # number of resources to be loaded
 *      0           # id of the first resource
 *      gin         # name of the first resource
 *
 * See the example resource database file provided in assets/resources.txt for
 * additional details on the file format.
 */
int load_resources_from_file(FILE *, Resource **);

/*
 * Frees a list of pointers to resources. Assumes list is len resources long.
 *
 * TODO: consider this a utility function. Maybe it should be somewhere else.
 */
void resource_list_destroy(Resource **, int len);
