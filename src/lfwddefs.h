
#ifndef __LAYA_FWD_DEFS_H__
#define __LAYA_FWD_DEFS_H__

#if defined(_cplusplus) || defined(__cplusplus)
extern "C" {
#endif

#include <inttypes.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "utils/lutils.h"

#define HWORD_SIZE  sizeof(HWORD)
#define REG_SIZE    sizeof(REG)
#define SWORD_SIZE  sizeof(SWORD)
#define DWORD_SIZE  sizeof(DWORD)

typedef struct LValueType LValueType;
typedef struct LFunctionType LFunctionType;
typedef struct LFunctionSignature LFunctionSignature;
typedef struct LShred LShred;
typedef struct LEvent LEvent;
typedef struct LUnitGenerator LUnitGenerator;
typedef struct LComplex LComplex;
typedef struct LDuration LDuration;
typedef struct LContainer LContainer;
typedef struct LRTObject LRTObject;
typedef struct LRTPrototype LRTPrototype;
typedef struct LModule LModule;
typedef struct LConstant LConstant;
typedef struct LTable LTable;
typedef struct LValue LValue;
typedef struct LVariable LVariable;
typedef struct LFunction LFunction;
typedef struct LInstance LInstance;
typedef struct LStackFrame LStackFrame;
typedef struct LVM LVM;

#if defined(_cplusplus) || defined(__cplusplus)
}
#endif

#endif

