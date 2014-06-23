
#ifndef __LAYA_RUNTIME_H__
#define __LAYA_RUNTIME_H__

#include "lstatic.h"

#if defined(_cplusplus) || defined(__cplusplus)
extern "C" {
#endif

/**
 * Prototype are the meta data behind runtime objects and their instance that are 
 * created/managed by the VM.
 */
struct LRTPrototype
{
    /**
     * Name of the container.
     */
    char *name;

    /**
     * The prototype this extends.
     */
    LRTPrototype *base;

    /**
     * The members functions of the instance of this prototype.
     */
    LArray *members;

    /**
     * The attributes of the instance of this prototype.
     */
    LArray *attributes;

    /**
     * Total space required for instance variables.
     * This will be determined at run time and will take into account
     * any overrides.
     */
    int total_attribute_size;
};

/**
 * An run time object instance.  The VM will create instances of these objects 
 * based on classes etc.
 */
struct LRTObject
{
    /**
     * Information on the class/type of object this is.
     */
    LRTPrototype *prototype;

    /**
     * The object's members.  The size of this would be cls->total_attribute_size.
     */
    SWORD members[1];
};

/**
 * Create a new prototype object.
 */
extern LRTPrototype *lrtprototype_create(const char *name);

/**
 * Destroy a prototype object and all its children.
 */
extern void lrtprototype_destroy(LRTPrototype *proto);

/**
 * Add a function to this prototype.
 */
extern void lrtprototype_add_member(LRTPrototype *proto, LFunction *function);

/**
 * Get the member at a given index.
 */
extern LFunction *lrtprototype_member_at_index(LRTPrototype *proto, int index);

/**
 * Get the number of members in this prototype.
 */
extern unsigned lrtprototype_member_count(LRTPrototype *proto);

#if defined(_cplusplus) || defined(__cplusplus)
}
#endif

#endif

