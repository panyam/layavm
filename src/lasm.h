
#ifndef __LAYAASM_H__
#define __LAYAASM_H__

#include "lfwddefs.h"

#if defined(_cplusplus) || defined(__cplusplus)
extern "C" {
#endif

/**
 * The bytecode assembler.  Converts module definition and bytecode in human readable form 
 * to compiled bytecode isntructions.  This is usually a result of the compiler, but this 
 * step could be skipped for direct bytecode generation.
 */
struct LASM
{
};

#if defined(_cplusplus) || defined(__cplusplus)
}
#endif

#endif

