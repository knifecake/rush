#include "resource.h"

#include "../lib/error_handling.h"

#include <stdlib.h>
#include <string.h>

struct _Resource{
  int id;
  char name[MAX_RESOURCE_NAME + 1];
};


Resource *resource_new(int id, const char *name)
{
    if (!name) {
        HE("cannot create resource, missing name", "resource_new")
        return NULL;
    }

    if (strlen(name) > MAX_RESOURCE_NAME) {
        HE("cannot create resource, name too long", "resource_new")
        return NULL;
    }

    Resource *r = malloc(sizeof(Resource));
    if (!r) {
        HE("cannot create resource, out of memory", "resource_new")
        return NULL;
    }

    r->id = id;
    strcpy(r->name, name);
    return r;
}

void resource_destroy(Resource *r)
{
    free(r);
}


void resource_print(FILE *s, Resource *r)
{
    if (!s || !r) {
        HE("invalid parameters", "resource_print")
        return;
    }

    fprintf(s, "Resource %s, id = %d\n", r->name, r->id);
}
