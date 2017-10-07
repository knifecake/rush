#include "resource.h"

#include "../error_handling.h"

#include <stdlib.h>
#include <string.h>

struct _Resource{
  int id;
  char name[MAX_RESOURCE_NAME + 1];
};


Resource *resource_new(int id, const char *name)
{
    if (!name) {
        handle_error("cannot create resource, missing name", "resource_new", __FILE__, __LINE__);
        return NULL;
    }

    if (strlen(name) > MAX_RESOURCE_NAME) {
        handle_error("cannot create resource, name too long", "resource_new", __FILE__, __LINE__);
        return NULL;
    }

    Resource *r = malloc(sizeof(Resource));
    if (!r) {
        handle_error("cannot create resource, out of memory", "resource_new", __FILE__, __LINE__);
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
        handle_error("invalid parameters", "resource_print", __FILE__, __LINE__);
        return;
    }

    fprintf(s, "Resource %s, id = %d\n", r->name, r->id);
}
