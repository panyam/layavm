
#include "layavm.h"

#define pc2reg(pc, reg)         { reg = *((REG *)pc); pc += REG_SIZE;           }
#define sval2memory(sval)       ((SWORD *)(intptr_t)(sval))
#define pc2memory(pc, memory)   { memory = (SWORD *)(intptr_t)(*((SWORD *)pc)); pc += SWORD_SIZE;  }
#define pc2hvalue(pc, hvalue)   { hvalue = *((HWORD *)pc); pc += HWORD_SIZE;    }
#define pc2svalue(pc, svalue)   { svalue = *((SWORD *)pc); pc += SWORD_SIZE;    }
#define pc2dvalue(pc, dvalue)   { dvalue = *((DWORD *)pc); pc += DWORD_SIZE;    }

void default_vm_initialise(LVM *vm);
void default_vm_finalise(LVM *vm);
void default_vm_run(LVM *vm);
void default_vm_dispatch(LVM *vm);
void default_vm_load_module(LVM *vm, const char *path);

/**
 * Creates a new VM.
 */
LVM *lvm_create()
{
    LVM *vm = ZNEW(LVM);
    vm->initialise = default_vm_initialise;
    vm->finalise = default_vm_finalise;
    vm->run = default_vm_run;
    vm->dispatch = default_vm_dispatch;
    vm->load_module = default_vm_load_module;
    return vm;
}

/**
 * Destroys a vm.
 */
void lvm_destroy(LVM *vm)
{
    if (vm)
    {
        vm->finalise(vm);
        free(vm);
    }
}

/**
 * Spork a function in the VM and create a new shred which will be added to the 
 * the VM's shred list for execution.
 * This method can take a variable number of arguments to indicate the parameters
 * to be passed to the function.  This list must be NULL terminated.
 */
LShred *lvm_spork(LVM *vm, LFunction *function, ...)
{
    LShred *shred = lshred_create(function);
    if (!vm->shreds)
        vm->shreds = llist_create();
    llist_append(vm->shreds, shred);
    return shred;
}

void default_vm_initialise(LVM *vm)
{
    vm->shreds = llist_create();
} 

void default_vm_finalise(LVM *vm)
{
}

void default_vm_run(LVM *vm)
{
    for (LListNode *shred = vm->shreds->head;shred != NULL;shred = shred->next)
    {
        LStackFrame *stack_frame = ((LShred *)shred->data)->call_stack;
        LFunction *function = stack_frame->function;
        unsigned char *program = function->program;
        unsigned char *pc = stack_frame->pc;
        SWORD *sregs = stack_frame->regs;
        DWORD *dregs = (DWORD *)(stack_frame->regs + function->num_sregs);
        REG src_reg, dest_reg;
        REG reg1, reg2;
        SWORD sword_value;
        DWORD dword_value;
        SWORD *memory;
        SWORD offset;
        FLOAT fv1, fv2;
        DOUBLE dv1, dv2;
        switch (*pc++)
        {
            case LOP_NOOP:
                break ;
            case LOP_HALT:
                break;
            case LOP_LOADS0:
                dest_reg = *((REG *)pc); pc += REG_SIZE;
                sregs[dest_reg] = 0;
                break;
            case LOP_LOADS1:
                dest_reg = *((REG *)pc); pc += REG_SIZE;
                sregs[dest_reg] = 1;
                break;
            case LOP_LOADSV:
                dest_reg = *((REG *)pc); pc += REG_SIZE;
                sword_value = *((SWORD *)pc); pc += SWORD_SIZE;
                sregs[dest_reg] = sword_value;
                break;
            case LOP_LOADSM:
                pc2reg(pc, dest_reg);
                pc2memory(pc, memory);
                sregs[dest_reg] = *memory;
                break;
            case LOP_LOADSMO:
                pc2reg(pc, dest_reg);
                pc2memory(pc, memory);
                pc2svalue(pc, offset);
                sregs[dest_reg] = memory[offset];
                break;
            case LOP_LOADSRO:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2svalue(pc, offset);
                memory = sval2memory(sregs[reg1]);
                sregs[dest_reg] = memory[offset];
                break;
            case LOP_STORESM:
                pc2reg(pc, src_reg);
                pc2memory(pc, memory);
                *memory = sregs[src_reg];
                break;
            case LOP_STORESMO:
                pc2reg(pc, src_reg);
                pc2memory(pc, memory);
                pc2svalue(pc, offset);
                memory[offset] = sregs[src_reg];
                break;
            case LOP_STORESRO:
                pc2reg(pc, src_reg);
                pc2reg(pc, reg1);
                memory = sval2memory(sregs[reg1]);
                pc2svalue(pc, offset);
                memory[offset] = sregs[src_reg];
                break;
            case LOP_LOADD0:
                pc2reg(pc, dest_reg);
                dregs[dest_reg] = 0;
                break;
            case LOP_LOADD1: dregs[*pc++] = 1;
                pc2reg(pc, dest_reg);
                dregs[dest_reg] = 1;
                break;
            case LOP_LOADDV:
                pc2reg(pc, dest_reg);
                pc2dvalue(pc, dword_value)
                dregs[dest_reg] = dword_value;
                break;
            case LOP_LOADDM:
                pc2reg(pc, dest_reg);
                pc2memory(pc, memory);
                dregs[dest_reg] = *((DWORD *)memory);
                break;
            case LOP_LOADDMO:
                pc2reg(pc, dest_reg);
                pc2memory(pc, memory);
                pc2svalue(pc, offset);
                dregs[dest_reg] = ((DWORD *)memory)[offset];
                break;
            case LOP_LOADDRO:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2svalue(pc, offset);
                memory = sval2memory(dregs[reg1]);
                dregs[dest_reg] = ((DWORD *)memory)[offset];
                break;
            case LOP_STOREDM:
                pc2reg(pc, src_reg);
                pc2memory(pc, memory);
                *((DWORD *)memory) = dregs[src_reg];
                break;
            case LOP_STOREDMO:
                pc2reg(pc, src_reg);
                pc2memory(pc, memory);
                pc2svalue(pc, offset);
                ((DWORD *)memory)[offset] = dregs[src_reg];
                break;
            case LOP_STOREDRO:
                pc2reg(pc, src_reg);
                pc2reg(pc, reg1);
                memory = sval2memory(sregs[reg1]);
                pc2svalue(pc, offset);
                ((DWORD *)memory)[offset] = dregs[src_reg];
                break;
            case LOP_COPYS:
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                sregs[reg1] = sregs[reg2];
                break;
            case LOP_COPYD:
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                dregs[reg1] = dregs[reg2];
                break;
            case LOP_I2F:
                pc2reg(pc, reg1); pc2reg(pc, reg2);
                ((FLOAT *)(sregs + reg1))[0] = sregs[reg2];
                break;
            case LOP_I2L:
                pc2reg(pc, reg1); pc2reg(pc, reg2);
                dregs[reg1] = sregs[reg2];
                break;
            case LOP_L2I:
                pc2reg(pc, reg1); pc2reg(pc, reg2);
                sregs[reg1] = dregs[reg2];
                break;
            case LOP_F2I:
                pc2reg(pc, reg1); pc2reg(pc, reg2);
                sregs[reg1] = (INT)((FLOAT *)(sregs + reg2))[0];
                break;
            case LOP_F2D:
                pc2reg(pc, reg1); pc2reg(pc, reg2);
                ((DOUBLE *)(dregs + reg1))[0] = ((FLOAT *)(sregs + reg2))[0];
                break;
            case LOP_D2F:
                pc2reg(pc, reg1); pc2reg(pc, reg2);
                ((FLOAT *)(sregs + reg1))[0] = ((DOUBLE *)(dregs + reg2))[0];
                break;
            case LOP_L2D:
                pc2reg(pc, reg1); pc2reg(pc, reg2);
                ((LONG *)(dregs + reg1))[0] = ((DOUBLE *)(dregs + reg2))[0];
                break;
            case LOP_D2L:
                pc2reg(pc, reg1); pc2reg(pc, reg2);
                ((DOUBLE *)(dregs + reg1))[0] = ((DOUBLE *)(dregs + reg2))[0];
                break;

            case LOP_ADDI:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                sregs[dest_reg] = sregs[reg1] + sregs[reg2];
                break;
            case LOP_SUBI:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                sregs[dest_reg] = sregs[reg1] - sregs[reg2];
                break;
            case LOP_MULI:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                sregs[dest_reg] = sregs[reg1] * sregs[reg2];
                break;
            case LOP_DIVI:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                sregs[dest_reg] = sregs[reg1] / sregs[reg2];
                break;
            case LOP_MODI:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                sregs[dest_reg] = sregs[reg1] % sregs[reg2];
                break;
            case LOP_NEGI:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                sregs[dest_reg] = -sregs[reg1];
                break;

            case LOP_ADDL:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                dregs[dest_reg] = dregs[reg1] + dregs[reg2];
                break;
            case LOP_SUBL:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                dregs[dest_reg] = dregs[reg1] - dregs[reg2];
                break;
            case LOP_MULL:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                dregs[dest_reg] = dregs[reg1] * dregs[reg2];
                break;
            case LOP_DIVL:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                dregs[dest_reg] = dregs[reg1] / dregs[reg2];
                break;
            case LOP_MODL:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                dregs[dest_reg] = dregs[reg1] % dregs[reg2];
                break;
            case LOP_NEGL:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                dregs[dest_reg] = -dregs[reg1];
                break;

            case LOP_ADDF:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                *((FLOAT *)(sregs + dest_reg)) = *((FLOAT *)(sregs + reg1)) + *((FLOAT *)(sregs + reg2));
                break;
            case LOP_SUBF:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                *((FLOAT *)(sregs + dest_reg)) = *((FLOAT *)(sregs + reg1)) - *((FLOAT *)(sregs + reg2));
                break;
            case LOP_MULF:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                *((FLOAT *)(sregs + dest_reg)) = *((FLOAT *)(sregs + reg1)) * *((FLOAT *)(sregs + reg2));
                break;
            case LOP_DIVF:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                *((FLOAT *)(sregs + dest_reg)) = *((FLOAT *)(sregs + reg1)) / *((FLOAT *)(sregs + reg2));
                break;
            case LOP_MODF:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                fv1 = *((FLOAT *)(sregs + reg1));
                fv2 = *((FLOAT *)(sregs + reg2));
                *((FLOAT *)(sregs + dest_reg)) = fv1 - (fv2 * (INT)(fv1 / fv2));
                break;
            case LOP_NEGF:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                *((FLOAT *)(sregs + dest_reg)) = -*((FLOAT *)(sregs + reg1));
                break;

            case LOP_ADDD:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                *((DOUBLE *)(dregs + dest_reg)) = *((DOUBLE *)(dregs + reg1)) + *((DOUBLE *)(dregs + reg2));
                break;
            case LOP_SUBD:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                *((DOUBLE *)(dregs + dest_reg)) = *((DOUBLE *)(dregs + reg1)) - *((DOUBLE *)(dregs + reg2));
                break;
            case LOP_MULD:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                *((DOUBLE *)(dregs + dest_reg)) = *((DOUBLE *)(dregs + reg1)) * *((DOUBLE *)(dregs + reg2));
                break;
            case LOP_DIVD:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                *((DOUBLE *)(dregs + dest_reg)) = *((DOUBLE *)(dregs + reg1)) / *((DOUBLE *)(dregs + reg2));
                break;
            case LOP_MODD:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                pc2reg(pc, reg2);
                dv1 = *((DOUBLE *)(dregs + reg1));
                dv2 = *((DOUBLE *)(dregs + reg2));
                *((DOUBLE *)(dregs + dest_reg)) = dv1 - (dv2 * (INT)(dv1 / dv2));
                break;
            case LOP_NEGD:
                pc2reg(pc, dest_reg);
                pc2reg(pc, reg1);
                *((DOUBLE *)(dregs + dest_reg)) = -*((DOUBLE *)(dregs + reg1));
                break;

            case LOP_ANDI:
                pc2reg(pc, dest_reg); pc2reg(pc, reg1); pc2reg(pc, reg2);
                sregs[dest_reg] = sregs[reg1] & sregs[reg2];
                break;
            case LOP_ORI:
                pc2reg(pc, dest_reg); pc2reg(pc, reg1); pc2reg(pc, reg2);
                sregs[dest_reg] = sregs[reg1] | sregs[reg2];
                break;
            case LOP_XORI:
                pc2reg(pc, dest_reg); pc2reg(pc, reg1); pc2reg(pc, reg2);
                sregs[dest_reg] = sregs[reg1] ^ sregs[reg2];
                break;
            case LOP_NOTI:
                pc2reg(pc, dest_reg); pc2reg(pc, reg1); 
                sregs[dest_reg] = ~sregs[reg1];
                break;
            case LOP_LSHIFTI:
                pc2reg(pc, dest_reg); pc2reg(pc, reg1); pc2reg(pc, reg2);
                sregs[dest_reg] = sregs[reg1] << sregs[reg2];
                break;
            case LOP_RSHIFTI:
                pc2reg(pc, dest_reg); pc2reg(pc, reg1); pc2reg(pc, reg2);
                sregs[dest_reg] = sregs[reg1] >> sregs[reg2];
                break;
            case LOP_ALSHIFTI:
                pc2reg(pc, dest_reg); pc2reg(pc, reg1); pc2reg(pc, reg2);
                sregs[dest_reg] = sregs[reg1] << sregs[reg2];
                break;
            case LOP_ARSHIFTI:
                pc2reg(pc, dest_reg); pc2reg(pc, reg1); pc2reg(pc, reg2);
                sregs[dest_reg] = sregs[reg1] >> sregs[reg2];
                break;

            case LOP_ANDL:
                pc2reg(pc, dest_reg); pc2reg(pc, reg1); pc2reg(pc, reg2);
                dregs[dest_reg] = dregs[reg1] & dregs[reg2];
                break;
            case LOP_ORL:
                pc2reg(pc, dest_reg); pc2reg(pc, reg1); pc2reg(pc, reg2);
                dregs[dest_reg] = dregs[reg1] | dregs[reg2];
                break;
            case LOP_XORL:
                pc2reg(pc, dest_reg); pc2reg(pc, reg1); pc2reg(pc, reg2);
                dregs[dest_reg] = dregs[reg1] ^ dregs[reg2];
                break;
            case LOP_NOTL:
                pc2reg(pc, dest_reg); pc2reg(pc, reg1); 
                dregs[dest_reg] = ~dregs[reg1];
                break;
            case LOP_LSHIFTL:
                pc2reg(pc, dest_reg); pc2reg(pc, reg1); pc2reg(pc, reg2);
                dregs[dest_reg] = dregs[reg1] << dregs[reg2];
                break;
            case LOP_RSHIFTL:
                pc2reg(pc, dest_reg); pc2reg(pc, reg1); pc2reg(pc, reg2);
                dregs[dest_reg] = dregs[reg1] >> dregs[reg2];
                break;
            case LOP_ALSHIFTL:
                pc2reg(pc, dest_reg); pc2reg(pc, reg1); pc2reg(pc, reg2);
                dregs[dest_reg] = dregs[reg1] << dregs[reg2];
                break;
            case LOP_ARSHIFTL:
                pc2reg(pc, dest_reg); pc2reg(pc, reg1); pc2reg(pc, reg2);
                dregs[dest_reg] = dregs[reg1] >> dregs[reg2];
                break;

            case LOP_BR:
                pc2svalue(pc, offset); pc = program + offset;
                break;
            case LOP_BREQS:
                pc2reg(pc, reg1); pc2reg(pc, reg2); pc2svalue(pc, offset);
                if (sregs[reg1] == sregs[reg2]) pc = program + offset;
                break;
            case LOP_BRNES:
                pc2reg(pc, reg1); pc2reg(pc, reg2); pc2svalue(pc, offset);
                if (sregs[reg1] != sregs[reg2]) pc = program + offset;
                break;
            case LOP_BREQD:
                pc2reg(pc, reg1); pc2reg(pc, reg2); pc2svalue(pc, offset);
                if (dregs[reg1] == dregs[reg2]) pc = program + offset;
                break;
            case LOP_BRNED:
                pc2reg(pc, reg1); pc2reg(pc, reg2); pc2svalue(pc, offset);
                if (dregs[reg1] != dregs[reg2]) pc = program + offset;
                break;
            case LOP_BREQS0:
                pc2reg(pc, reg1); pc2svalue(pc, offset);
                if (sregs[reg1] == 0) pc = program + offset;
                break;
            case LOP_BRNES0:
                pc2reg(pc, reg1); pc2svalue(pc, offset);
                if (sregs[reg1] != 0) pc = program + offset;
                break;
            case LOP_BREQD0:
                pc2reg(pc, reg1); pc2svalue(pc, offset);
                if (dregs[reg1] == 0) pc = program + offset;
                break;
            case LOP_BRNED0:
                pc2reg(pc, reg1); pc2svalue(pc, offset);
                if (dregs[reg1] != 0) pc = program + offset;
                break;

            case LOP_BRLTI:
                pc2reg(pc, reg1); pc2svalue(pc, offset);
                if (sregs[reg1] < sregs[reg2]) pc = program + offset;
                break;
            case LOP_BRLEI:
                pc2reg(pc, reg1); pc2svalue(pc, offset);
                if (sregs[reg1] <= sregs[reg2]) pc = program + offset;
                break;

            case LOP_BRLTL:
                pc2reg(pc, reg1); pc2svalue(pc, offset);
                if (dregs[reg1] < dregs[reg2]) pc = program + offset;
                break;
            case LOP_BRLEL:
                pc2reg(pc, reg1); pc2svalue(pc, offset);
                if (dregs[reg1] <= dregs[reg2]) pc = program + offset;
                break;

            case LOP_BRLTF:
                pc2reg(pc, reg1); pc2svalue(pc, offset);
                if (*((FLOAT *)(sregs + reg1)) < *((FLOAT *)(sregs + reg2))) pc = program + offset;
                break;
            case LOP_BRLEF:
                pc2reg(pc, reg1); pc2svalue(pc, offset);
                if (*((FLOAT *)(sregs + reg1)) <= *((FLOAT *)(sregs + reg2))) pc = program + offset;
                break;

            case LOP_BRLTD:
                pc2reg(pc, reg1); pc2svalue(pc, offset);
                if (*((DOUBLE *)(dregs + reg1)) < *((DOUBLE *)(dregs + reg2))) pc = program + offset;
                break;
            case LOP_BRLED:
                pc2reg(pc, reg1); pc2svalue(pc, offset);
                if (*((DOUBLE *)(dregs + reg1)) <= *((DOUBLE *)(dregs + reg2))) pc = program + offset;
                break;
        }
    }
} 

void default_vm_dispatch(LVM *vm)
{
}

void default_vm_load_module(LVM *vm, const char *path)
{
} 

/**
 * Creates a new shred starting from a given function.
 * \param function  Function the shred will begin executing on.
 * \return A pointer to the new LShred instance.
 */
LShred *lshred_create(LFunction *function)
{
    LShred *shred = ZNEW(LShred);
    LStackFrame *frame = (LStackFrame *)malloc(sizeof(LStackFrame) + (sizeof(SWORD) * function->num_sregs) + (sizeof(DWORD) * function->num_dregs));
    frame->function = function;
    frame->pc = function->program;
    shred->call_stack = frame;
    return shred;
}

