#include <interface99.h>

#include "../common.h"
#include "../util.h"
#include "types.h"

#include <assert.h>

declImplExtern(Foo, FooImpl);

// Multiple declarations shoud work fine.
declImplExtern(Bar, BarImpl);
declImplExtern(Bar, BarImpl);

int main(void) {
    // Ensure `impl`-generated data.
    {
        assert(VTABLE(FooImpl, Foo).foo == foo1_impl);
        assert(VTABLE(BarImpl, Bar).foo == foo1_impl);
    }

    ENSURE_DYN_OBJ(FooImpl, Foo);
    ENSURE_DYN_OBJ(BarImpl, Bar);

    return 0;
}
