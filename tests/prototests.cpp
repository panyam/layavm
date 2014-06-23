
#include <UnitTest++.h>
#include "layavm.h"

class PrototypeTestFixture
{
public:
    PrototypeTestFixture()
    {
    }

    virtual ~PrototypeTestFixture()
    {
    }
};

/**
 * Create a basic prototype.
 */
TEST_FIXTURE(PrototypeTestFixture, TestBTCreate)
{
    LRTPrototype *proto = lrtprototype_create("module:test");
    CHECK(proto != NULL);
    lrtprototype_destroy(proto);
}

/**
 * Add a function to the prototype.
 */
TEST_FIXTURE(PrototypeTestFixture, TestAddMember)
{
    LRTPrototype *proto = lrtprototype_create("module:test");
    // lrtprototype_add_member(proto, function);
    CHECK(proto != NULL);
    lrtprototype_destroy(proto);
}

