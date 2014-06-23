

#ifndef __LVM_INSTR_H__
#define __LVM_INSTR_H__

enum LErrorCode
{
    LERROR_OK,
    LERROR_OUT_OF_MEMORY
};

/**
 * The instruction set for the Laya VM.  We want to start simple first.
 *
 */
enum LOpCode 
{
    LOP_NOOP,     // No-op
    LOP_HALT,     // Halt the running program

    /**
     * Single word register load instructions
     */
    LOP_LOADS0,   // Load 0 into a given single word register
    LOP_LOADS1,   // Load 1 into a given single word register
    LOP_LOADSV,   // Load a word sized value into a single word register
    LOP_LOADSM,   // Load a word from a given memory location into a single word register
    LOP_LOADSMO,  // Load a word from an offset from a given memory location into a single word register
    LOP_LOADSRO,  // Load a word from an offset from a given memory location stored in a register into a single word register

    /**
     * Single word register store instructions
     */
    LOP_STORESM,   // Store a single-word register into a given Memory location
    LOP_STORESMO,  // Store a single-word register into an offset from a given memory location
    LOP_STORESRO,  // Store a single-word register into an offset from a given Memory location stored in a register

    /**
     * Double word register load instructions
     */
    LOP_LOADD0,   // Load 0 into a given double word register
    LOP_LOADD1,   // Load 1 into a given double word register
    LOP_LOADDV,   // Load a word sized value into a double word register
    LOP_LOADDM,   // Load a word from a given memory location into a double word register
    LOP_LOADDMO,  // Load a word from an offset from a given memory location into a double word register
    LOP_LOADDRO,  // Load a word from an offset from a given memory location stored in a register into a double word register

    /**
     * Double word register store instructions
     */
    LOP_STOREDM,   // Store a double-word register into a given Memory location
    LOP_STOREDMO,  // Store a double-word register into an offset from a given memory location
    LOP_STOREDRO,  // Store a double-word register into an offset from a given Memory location stored in a register

    /**
     * Register move and casting instructions.
     */
    LOP_COPYS,    // Copy between single word registers
    LOP_COPYD,    // Copy between double word registers
    LOP_I2F,      // integer to float
    LOP_I2L,      // integer to long
    LOP_L2I,      // long to integer
    LOP_F2I,      // float to integer
    LOP_F2D,      // float to double
    LOP_D2F,      // double to float
    LOP_L2D,      // long to double
    LOP_D2L,      // double to long

    /**
     * Arithmetic instructions.
     */
    LOP_ADDI,     // Add 2 integers and store into single word register
    LOP_SUBI,     // Subtract 2 integers and store into single word register
    LOP_MULI,     // Multiply 2 integers and store into single word register
    LOP_DIVI,     // Divide 2 integers and store into single word register
    LOP_MODI,     // Integer module and store into single word register
    LOP_NEGI,     // Negate an integer and store into single word register

    LOP_ADDL,     // Add 2 longs and store into single word register
    LOP_SUBL,     // Subtract 2 longs and store into single word register
    LOP_MULL,     // Multiply 2 longs and store into single word register
    LOP_DIVL,     // Divide 2 longs and store into single word register
    LOP_MODL,     // long module and store into single word register
    LOP_NEGL,     // Negate an long and store into single word register

    LOP_ADDF,     // Add 2 floats and store into single word register
    LOP_SUBF,     // Subtract 2 floats and store into single word register
    LOP_MULF,     // Multiply 2 floats and store into single word register
    LOP_DIVF,     // Divide 2 floats and store into single word register
    LOP_MODF,     // float module and store into single word register
    LOP_NEGF,     // Negate an float and store into single word register

    LOP_ADDD,     // Add 2 doubles and store into single word register
    LOP_SUBD,     // Subtract 2 doubles and store into single word register
    LOP_MULD,     // Multiply 2 doubles and store into single word register
    LOP_DIVD,     // Divide 2 doubles and store into single word register
    LOP_MODD,     // double module and store into single word register
    LOP_NEGD,     // Negate an double and store into single word register

    /**
     * Bitwise instructions
     */
    LOP_ANDI,     // Bitwise AND of two integers and store result into single word integer
    LOP_ORI,      // Bitwise OR of two integers and store result into single word integer
    LOP_XORI,     // Bitwise XOR of two integers and store result into single word integer
    LOP_NOTI,     // Bitwise NOT of an integer and store result into single word integer
    LOP_LSHIFTI,  // Bitwise left shift of an integer and store result into single word integer
    LOP_RSHIFTI,  // Bitwise right shift of an integer and store result into single word integer
    LOP_ALSHIFTI, // Arithmetic left shift of an integer and store result into single word integer
    LOP_ARSHIFTI, // Arithmetic right shift of an integer and store result into single word integer

    LOP_ANDL,     // Bitwise AND of two longs and store result into single word long
    LOP_ORL,      // Bitwise OR of two longs and store result into single word long
    LOP_XORL,     // Bitwise XOR of two longs and store result into single word long
    LOP_NOTL,     // Bitwise NOT of an long and store result into single word long
    LOP_LSHIFTL,  // Bitwise left shift of an long and store result into single word long
    LOP_RSHIFTL,  // Bitwise right shift of an long and store result into single word long
    LOP_ALSHIFTL, // Arithmetic left shift of an long and store result into single word long
    LOP_ARSHIFTL, // Arithmetic right shift of an long and store result into single word long

    /**
     * Branch instructions
     */
    LOP_BR,       // Unconditional branch to offset
    LOP_BREQS,    // Branch to offset if rega == regb (single words)
    LOP_BRNES,    // Branch to offset if rega != regb (single words)
    LOP_BREQD,    // Branch to offset if rega == regb (double words)
    LOP_BRNED,    // Branch to offset if rega != regb (double words)
    LOP_BREQS0,   // Branch to offset if rega == 0 (single words)
    LOP_BRNES0,   // Branch to offset if rega != 0 (single words)
    LOP_BREQD0,   // Branch to offset if rega == 0 (double words)
    LOP_BRNED0,   // Branch to offset if rega != 0 (double words)

    LOP_BRLTI,    // Branch to offset if rega < regb (ints)
    LOP_BRLEI,    // Branch to offset if rega <= regb (ints)

    LOP_BRLTL,    // Branch to offset if rega < regb (longs)
    LOP_BRLEL,    // Branch to offset if rega <= regb (longs)

    LOP_BRLTF,    // Branch to offset if rega < regb (floats)
    LOP_BRLEF,    // Branch to offset if rega <= regb (floats)

    LOP_BRLTD,    // Branch to offset if rega < regb (doubles)
    LOP_BRLED,    // Branch to offset if rega <= regb (doubles)

    LOP_COUNT
};

#endif

