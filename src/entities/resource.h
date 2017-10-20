#ifndef __RESOURCE_H__
#define __RESOURCE_H__

/*
 * resource.h
 *
 * A resource that belongs to the player.
 *
 */


#include <stdio.h>
#include <stdlib.h>

/*
 * The maximun number of Resources the game currently supports.
 *
 * Note that other entities such as tile depend on this constant.
 * Choosing a very large number and not using that many Resources
 * means a lot of wasted space in other entities.
 */
#define MAX_RESOURCES 5


/*
 * Maximun length of the resource name.
 */
#define MAX_RESOURCE_NAME 32

typedef struct _Resource Resource;

/*
 * Creates a resource with the given ID and name. Name is copied on
 * creation of the resource and freed on deletion.
 *
 * Returns NULL on error.
 */
Resource *resource_new(int , const char *);


/*
 * Frees the given resource.
 */
void resource_destroy(Resource *);

/*
 * Prints the given resource, for debugging purposes.
 */
void resource_print(FILE *, Resource *);

#endif
