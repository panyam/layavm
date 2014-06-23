
#include <UnitTest++.h>
#include "layavm.h"

class VMTestFixture
{
public:
    VMTestFixture()
    {
    }

    virtual ~VMTestFixture()
    {
    }
};

/**
 * Tests the creation of a VM.
 * Should have no shreds.  By default a shred is only created when a function is sporked.
 */
TEST_FIXTURE(VMTestFixture, TestBTCreate)
{
    LVM *vm = lvm_create();
    CHECK(vm != NULL);
    CHECK(vm->shreds == NULL);
    lvm_destroy(vm);
}

/**
 * Sporking should create a shred.
 */
TEST_FIXTURE(VMTestFixture, TestSporkFunction)
{
    LVM *vm = lvm_create();
    LFunction *func = lfunction_create("test");
    LShred *shred = lvm_spork(vm, func);
    CHECK(shred != NULL);
    CHECK(vm->shreds != NULL);
    CHECK(shred->call_stack != NULL);
    lvm_destroy(vm);
}
