
#include <UnitTest++.h>
#include "layavm.h"

class FunctionTestFixture
{
public:
    FunctionTestFixture() { }
    virtual ~FunctionTestFixture() { }
};

/**
 * Create a function.
 */
TEST_FIXTURE(FunctionTestFixture, TestCreate)
{
    LFunction *func= lfunction_create("test");
    CHECK(func != NULL);
    lfunction_destroy(func);
}

/**
 * Add NULL code to a function.
 * Should not change function's program size.
 */
TEST_FIXTURE(FunctionTestFixture, TestFunctionWithNullCode)
{
    LFunction *func= lfunction_create("test");
    int pl = func->proglen;
    CHECK(pl == 0);
    int result = lfunction_add_instructions(func, NULL, 0);
    CHECK(result == 0);
    CHECK(func->proglen == 0);
    lfunction_destroy(func);
}

/**
 * Add some code to a function.
 * Should update function's program size to codelen.
 */
TEST_FIXTURE(FunctionTestFixture, TestFunctionWithCode)
{
    LFunction *func= lfunction_create("test");
    unsigned char instr[256] = {
        LOP_LOADS0,
        LOP_LOADS1,
        LOP_LOADSV,
        LOP_LOADSM,
        LOP_LOADSMO,
        LOP_LOADSRO,
    };
    size_t codelen = 6;
    int result = lfunction_add_instructions(func, instr, codelen);
    CHECK(result == 0);
    CHECK(func->program != NULL);
    CHECK(func->proglen == codelen);
    CHECK_ARRAY_EQUAL(func->program, instr, codelen);
    lfunction_destroy(func);
}

/**
 * Add some code to a function twice.
 * Should update function's program size to codelen * 2
 */
TEST_FIXTURE(FunctionTestFixture, TestFunctionWithCodeMultiple)
{
    LFunction *func= lfunction_create("test");
    unsigned char instr[256] = {
        LOP_LOADS0,
        LOP_LOADS1,
        LOP_LOADSV,
        LOP_LOADSM,
        LOP_LOADSMO,
        LOP_LOADSRO,
    };
    size_t codelen = 6;
    int result = lfunction_add_instructions(func, instr, codelen);
    CHECK(result == 0);
    result = lfunction_add_instructions(func, instr, codelen);
    CHECK(result == 0);

    CHECK(func->program != NULL);
    CHECK(func->proglen == codelen * 2);
    CHECK_ARRAY_EQUAL(func->program, instr, codelen);
    CHECK_ARRAY_EQUAL(func->program + codelen, instr, codelen);
    lfunction_destroy(func);
}

