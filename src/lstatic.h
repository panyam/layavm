
#ifndef __LAYA_STATIC_H__
#define __LAYA_STATIC_H__

#include "lfwddefs.h"

#if defined(_cplusplus) || defined(__cplusplus)
extern "C" {
#endif

enum LType
{
    LT_NIL,
    LT_BOOL,
    LT_INT,
    LT_LONG,
    LT_FLOAT,
    LT_DOUBLE,
    LT_PTR,
    LT_TIME,
    LT_DURATION,
    LT_COMPLEX,
    LT_POLAR,
    LT_STRING,
    LT_EVENT,
    LT_UGEN,
    LT_TABLE,
    LT_OBJECT,
    LT_CLASS,
    LT_FUNCTION,
    LT_SHRED,
    LT_TYPE_COUNT,
};

struct LValueType
{
    enum LType valueType;
    union {
        LValueType *aliasType;
        LContainer *classType;
        LFunctionType *functionType;
    } value;
};

struct LFunctionType
{
    LValueType returnType;
    LValueType *paramTypes;
    int num_params;
    BOOL has_var_args;
};

struct LFunctionSignature
{
    LString *fname;
    LFunctionType ftype;
};

/**
 * An entry in the constant table.
 */
enum LConstantType
{
    CONST_INT,
    CONST_FLOAT,
    CONST_LONG,
    CONST_DOUBLE,
    CONST_COMPLEX,
    CONST_POLAR,
    CONST_TIME,
    CONST_DURATION,
    CONST_STRING,
    CONST_REFERENCE,
    MAX_CONST_TYPES
};

struct LFunction
{
    /**
     * Name of the function
     */
    char *name;

    /**
     * To allow for overloading.
     */
    const LFunctionType *functionType;

    /**
     * Whether the function should be responsible for balancing the stack.
     */
    BOOL balance_stack;

    /**
     * Total space required for all local variables.
     * Will be determined at parse time based on scope rules.
     */
    int num_sregs;
    int num_dregs;

    /**
     * Size of the function's program.
     */
    size_t proglen, progcapacity;

    /**
     * Opcodes corresponding to the function.
     */
    unsigned char *program;
};

struct LValue
{
    /**
     * Type of the value.
     */
    LValueType value_type;
    union
    {
        int intValue;
        long longValue;
        float floatValue;
        double doubleValue;
        LEvent *eventValue;
        LUnitGenerator *ugenValue;
        LFunction *funcValue;
        LShred *shredValue;
        LObject *objValue;
        LDuration *durationValue;
        LComplex *complexValue;
        LTable *tableValue;
    } value;
};

/**
 * Creates a new function.
 */
extern LFunction *lfunction_create(const char *name);

/**
 * Destroys a function.
 */
extern void lfunction_destroy(LFunction *function);

/**
 * Adds a set of instructions to a function's code body.
 *
 * \param   function  Function to add code to.
 * \param   code      The code bytes to be added.
 * \param   codelen   The number of bytes in the body to be added.
 *
 * \return  0 on success
 */
extern int lfunction_add_instructions(LFunction *function, unsigned char *code, size_t codelen);

extern LValue lvalue_int(int value);
extern LValue lvalue_long(long value);
extern LValue lvalue_float(float value);
extern LValue lvalue_double(double value);
extern LValue lvalue_event(void *event);
extern LValue lvalue_ugen(void *ugen);
extern LValue lvalue_function(LFunction *value);
extern LValue lvalue_shred(LShred *value);
extern LValue lvalue_rtobject(LRTObject *value);
extern LValue lvalue_duration(void *value);
extern LValue lvalue_complex(void *value);
extern LValue lvalue_table(LTable *value);

#if defined(_cplusplus) || defined(__cplusplus)
}
#endif

#endif

