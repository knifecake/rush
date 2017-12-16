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

    Resource *r = oopsalloc(1, sizeof(Resource), "resource_new");

    r->id = id;
    strcpy(r->name, name);
    return r;
}

void resource_destroy(Resource *r)
{
    free(r);
}

char *resource_get_name(Resource *r)
{
    if (!r) {
        HE("invalid arguments", "resource_get_name");
        return NULL;
    }

    return r->name;
}

void resource_print(FILE *s, Resource *r)
{
    if (!s || !r) {
        HE("invalid parameters", "resource_print")
        return;
    }

    fprintf(s, "Resource %s, id = %d\n", r->name, r->id);
}
