
#ifndef __LAYAVM_H__
#define __LAYAVM_H__

#include "lruntime.h"

#if defined(_cplusplus) || defined(__cplusplus)
extern "C" {
#endif

/**
 * Index into the constant pool.
 */
typedef int LConstantPoolIndex;

struct LEvent
{
    const char *name;
};

struct LUnitGenerator
{
    int a;
};

struct LComplex { double a, b; };

struct LDuration
{
    float value;
    LDuration *baseDuration;
};

/**
 * Tables.  These can be both associative or arrays.
 * This idea is taken from lua so the programmer does not have to worry about
 * sizes or dimensions.
 */
struct LTable
{
    /**
     * Length of the table - ie number of items.  Only relates to the array part of the table.
     */
    int length;

    /**
     * Number of "keys" in this table.
     */
    int num_keys;

    /**
     * The type of value.  Generic table if nil.
     */
    LValueType value_type;
};

struct LConstant
{
    unsigned char const_type;
};

struct LStackFrame
{
    /**
     * The parent from which this call was executed.
     */
    LStackFrame *parent;

    /**
     * Current function being executed.
     */
    LFunction *function;

    /**
     * Program counter corresponding to the function being executed.
     */
    unsigned char *pc;

    /**
     * All the regstiers used in this stack frame.
     * The size of this will be function.total_local_size.
     */
    SWORD regs[1];
};

struct LShred
{
    /**
     * ID of the shred assigned by the VM upon creation.
     */
    const int shred_id;

    /**
     * Name of the shred if any.
     */
    char *name;

    /**
     * The head of the call stack.
     */
    LStackFrame *call_stack;
};

struct LVM
{
    /**
     * Called to initialise a VM after it has been allocated.
     */
    void (*initialise)(LVM *vm);

    /**
     * Called to clean up and finalise the VM when it is being destroyed
     * and deallocated.
     */
    void (*finalise)(LVM *vm);

    /**
     * Runs a function within the VM.
     * This is the starting point of execution of a piece of code.
     */
    void (*run)(LVM *vm);

    /**
     * The dispatcher method that fetches/decodes/executes the byte codes.
     * This allows for custom implementation of the dispatcher. By default
     * the switch based dispatcher (SBD) is used for simplicity and maximum 
     * portability at the expense of execution speed.
     */
    void (*dispatch)(LVM *vm);

    /**
     * Loads a module given an absolute path to the file.
     */
    void (*load_module)(LVM *vm, const char *path);

    /**
     * All the shreds maintained by this VM and its scheduler sorted by order of priority.
     */
    LList *shreds;
    LLock *shred_list_lock;
    unsigned max_shred_id;
};

/**
 * Creates a new VM.
 */
extern LVM *lvm_create();

/**
 * Destroys a vm.
 */
extern void lvm_destroy(LVM *vm);

/**
 * Spork a function in the VM and create a new shred which will be added to the 
 * the VM's shred list for execution.
 * This method can take a variable number of arguments to indicate the parameters
 * to be passed to the function.  This list must be NULL terminated.
 */
extern LShred *lvm_spork(LVM *vm, LFunction *function, ...);

/**
 * Creates a new shred starting from a given function.
 * \param function  Function the shred will begin executing on.
 * \return A pointer to the new LShred instance.
 */
extern LShred *lshred_create(LFunction *function);

#if defined(_cplusplus) || defined(__cplusplus)
}
#endif

#endif

