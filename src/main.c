
#include "layavm.h"

int main(int argc, char **argv)
{
    LVM *vm = lvm_create();
    lvm_destroy(vm);
    return 0;
}

