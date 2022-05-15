#include <interface99.h>

#include "common.h"
#include "util.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

// Ensure that forward declarations are generated.
#define TestForwardDecl_IFACE vfunc(void, abc, Foo self, FooVTable vtable)

interface(TestForwardDecl);

// Implementations {

typedef struct {
    char dummy;
} MarkerImpl;

impl(Marker, MarkerImpl);

typedef struct {
    char dummy;
} FooImpl;

#define FooImpl_foo foo1_impl
impl(Foo, FooImpl);

typedef struct {
    char dummy;
} BarImpl1;

#define BarImpl1_foo foo1_impl
#define BarImpl1_bar bar1_impl
impl(Bar, BarImpl1);

// Ensure that an interface can be implemented by many types.
typedef struct {
    char dummy;
} BarImpl2;

#define BarImpl2_foo foo2_impl
#define BarImpl2_bar bar2_impl
impl(Bar, BarImpl2);

// Ensure that a type can implement multiple interfaces.
typedef struct {
    char dummy;
} FooBarImpl;

#define FooBarImpl_foo foo1_impl
impl(Foo, FooBarImpl);
#undef FooBarImpl_foo

#define FooBarImpl_foo foo1_impl
#define FooBarImpl_bar bar1_impl
impl(Bar, FooBarImpl);
#undef FooBarImpl_foo
#undef FooBarImpl_bar

typedef struct {
    int x;
    long long d;
    const char *str;
} TestCompoundLit;

#define TestCompoundLit_foo foo1_impl
impl(Foo, TestCompoundLit);
// } (Implementations)

int main(void) {
    // Ensure `interface`-generated data.
    {
        ENSURE_VTABLE_FIELD_TYPE(MarkerVTable, dummy, char);
        ENSURE_DYN_OBJ_TYPE(Marker);

        ENSURE_VTABLE_FIELD_TYPE(FooVTable, foo, FooOpType);
        ENSURE_DYN_OBJ_TYPE(Foo);

        ENSURE_VTABLE_FIELD_TYPE(BarVTable, foo, FooOpType);
        ENSURE_VTABLE_FIELD_TYPE(BarVTable, bar, BarOpType);
        ENSURE_DYN_OBJ_TYPE(Bar);
    }

    // Ensure `impl`-generated data.
    {
        assert(VTABLE(MarkerImpl, Marker).dummy == '\0');

        assert(VTABLE(FooImpl, Foo).foo == foo1_impl);

        assert(VTABLE(BarImpl1, Bar).foo == foo1_impl);
        assert(VTABLE(BarImpl1, Bar).bar == bar1_impl);

        assert(VTABLE(BarImpl2, Bar).foo == foo2_impl);
        assert(VTABLE(BarImpl2, Bar).bar == bar2_impl);

        assert(VTABLE(FooBarImpl, Foo).foo == foo1_impl);
        assert(VTABLE(FooBarImpl, Bar).foo == foo1_impl);
        assert(VTABLE(FooBarImpl, Bar).bar == bar1_impl);
    }

    ENSURE_DYN_OBJ(MarkerImpl, Marker);
    ENSURE_DYN_OBJ(FooImpl, Foo);
    ENSURE_DYN_OBJ(BarImpl1, Bar);
    ENSURE_DYN_OBJ(BarImpl2, Bar);
    ENSURE_DYN_OBJ(FooBarImpl, Foo);
    ENSURE_DYN_OBJ(FooBarImpl, Bar);

    // Test compound literals with `DYN_LIT`.
    {
        Foo compound = DYN_LIT(TestCompoundLit, Foo, {.x = 123, .d = 15, .str = "abc"});

        assert(compound.vptr == &VTABLE(TestCompoundLit, Foo));
        assert(((TestCompoundLit *)compound.self)->x == 123);
        assert(((TestCompoundLit *)compound.self)->d == 15);
        assert(strcmp(((TestCompoundLit *)compound.self)->str, "abc") == 0);
    }

    return 0;
}
