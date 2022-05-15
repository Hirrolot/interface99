#include <interface99.h>

#include "common.h"
#include "util.h"

#include <assert.h>

// Interfaces {

#define FooExtendsMarker_IFACE   Foo_IFACE
#define FooExtendsMarker_EXTENDS (Marker)

#define FooExtendsMany_IFACE   Foo_IFACE
#define FooExtendsMany_EXTENDS (Foo, Bar)

#define MarkerExtendsMarker_IFACE   Marker_IFACE
#define MarkerExtendsMarker_EXTENDS (Marker)

#define MarkerExtendsMany_IFACE   Marker_IFACE
#define MarkerExtendsMany_EXTENDS (Foo, Bar)

interface(FooExtendsMarker);
interface(FooExtendsMany);

interface(MarkerExtendsMarker);
interface(MarkerExtendsMany);
// } (Interfaces)

// FooExtendsMarker {

typedef struct {
    char dummy;
} A;

impl(Marker, A);

#define A_foo foo1_impl
impl(FooExtendsMarker, A);
// } (FooExtendsMarker)

// FooExtendsMany {

typedef struct {
    char dummy;
} B;

#define B_foo foo1_impl
impl(Foo, B);
#undef B_foo

#define B_foo foo1_impl
#define B_bar bar1_impl
impl(Bar, B);
#undef B_foo
#undef B_bar

#define B_foo foo1_impl
impl(FooExtendsMany, B);
#undef B_foo
// } (FooExtendsMany)

// MarkerExtendsMarker {

typedef struct {
    char dummy;
} C;

impl(Marker, C);
impl(MarkerExtendsMarker, C);
// } (MarkerExtendsMarker)

// MarkerExtendsMany {

typedef struct {
    char dummy;
} D;

#define D_foo foo1_impl
impl(Foo, D);
#undef D_foo

#define D_foo foo1_impl
#define D_bar bar1_impl
impl(Bar, D);
#undef D_foo
#undef D_bar

impl(MarkerExtendsMany, D);
// } (MarkerExtendsMany)

int main(void) {
    // Ensure `interface`-generated data.
    {
        ENSURE_VTABLE_FIELD_TYPE(FooExtendsMarkerVTable, foo, FooOpType);
        ENSURE_DYN_OBJ_TYPE(FooExtendsMarker);

        ENSURE_VTABLE_FIELD_TYPE(FooExtendsManyVTable, foo, FooOpType);
        ENSURE_DYN_OBJ_TYPE(FooExtendsMany);

        ENSURE_DYN_OBJ_TYPE(MarkerExtendsMarker);

        ENSURE_DYN_OBJ_TYPE(MarkerExtendsMany);
    }

    // Ensure `impl`-generated data.
    {
        assert(VTABLE(A, FooExtendsMarker).Marker == &VTABLE(A, Marker));

        assert(VTABLE(B, FooExtendsMany).Foo == &VTABLE(B, Foo));
        assert(VTABLE(B, FooExtendsMany).Bar == &VTABLE(B, Bar));

        assert(VTABLE(C, MarkerExtendsMarker).Marker == &VTABLE(C, Marker));

        assert(VTABLE(D, MarkerExtendsMany).Foo == &VTABLE(D, Foo));
        assert(VTABLE(D, MarkerExtendsMany).Bar == &VTABLE(D, Bar));
    }

    ENSURE_DYN_OBJ(A, FooExtendsMarker);
    ENSURE_DYN_OBJ(B, FooExtendsMany);
    ENSURE_DYN_OBJ(C, MarkerExtendsMarker);
    ENSURE_DYN_OBJ(D, MarkerExtendsMany);

    // Ensure that dynamic objects of superinterfaces still work.
    {
        ENSURE_DYN_OBJ(A, Marker);

        ENSURE_DYN_OBJ(B, Foo);
        ENSURE_DYN_OBJ(B, Bar);

        ENSURE_DYN_OBJ(C, Marker);

        ENSURE_DYN_OBJ(D, Foo);
        ENSURE_DYN_OBJ(D, Bar);
    }

    return 0;
}
