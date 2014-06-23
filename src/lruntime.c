
#include "layavm.h"

/**
 * Create a new prototype object.
 */
LRTPrototype *lrtprototype_create(const char *name)
{
    LRTPrototype *proto = (LRTPrototype *)calloc(1, sizeof(LRTPrototype));
    proto->name = strdup(name);
    return proto;
}

/**
 * Destroy a prototype object and all its children.
 */
void lrtprototype_destroy(LRTPrototype *proto)
{
    if (proto)
    {
        if (proto->name) free(proto->name);
        larray_destroy(proto->members, free);
        free(proto);
    }
}

