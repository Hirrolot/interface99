#include <interface99.h>

#include "common.h"

#include <assert.h>

typedef struct {
    char dummy;
} BarImpl;

declImpl(Bar, BarImpl);

static void test_decl_impl(void) {
    assert(VTABLE(BarImpl, Bar).foo == foo1_impl);
    assert(VTABLE(BarImpl, Bar).bar == bar1_impl);
}

#define BarImpl_foo foo1_impl
#define BarImpl_bar bar1_impl
impl(Bar, BarImpl);

// Multiple declarations should work fine.
declImpl(Bar, BarImpl);
declImpl(Bar, BarImpl);

int main(void) {
    test_decl_impl();

    return 0;
}
