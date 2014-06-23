
#include "layavm.h"

/**
 * Creates a new function.
 */
LFunction *lfunction_create(const char *name)
{
    LFunction *function = ZNEW(LFunction);
    function->name = strdup(name);
    return function;
}

/**
 * Destroys a function.
 */
void lfunction_destroy(LFunction *function)
{
    if (function)
    {
        free(function->name);
        free(function->program);
        free(function);
    }
}

/**
 * Adds a set of instructions to a function's code body.
 *
 * \param   code      The code bytes to be added.
 * \param   codelen   The number of bytes in the body to be added.
 *
 * \return  0 on success
 */
int lfunction_add_instructions(LFunction *function, unsigned char *code, size_t codelen)
{
    if (!function || !code) return 0;

    size_t newcapacity = function->proglen + codelen;
    if (function->progcapacity < newcapacity)
    {
        if ((function->program = realloc(function->program, newcapacity)) == NULL)
        {
            assert("Out of memory");
            return LERROR_OUT_OF_MEMORY;
        }
        function->progcapacity = newcapacity;
    }
    memcpy(function->program + function->proglen, code, codelen);
    function->proglen += codelen;
    return LERROR_OK;
}

