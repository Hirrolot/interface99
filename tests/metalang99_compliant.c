#include <interface99.h>

#include "util.h"

#include <metalang99.h>

#include <assert.h>
#include <stddef.h>

ML99_ASSERT_UNEVAL(IFACE99_interface_ARITY == 1);
ML99_ASSERT_UNEVAL(IFACE99_impl_ARITY == 2);
ML99_ASSERT_UNEVAL(IFACE99_implExtern_ARITY == 2);

#define Foo_IFACE vfunc(const char *, foo, int x, double *restrict y)

ML99_EVAL(IFACE99_interface(v(Foo)));

// Implementations {

static const char *foo_impl(int x, double *restrict y) {
    (void)x;
    (void)y;
    return (const char *)NULL;
}

typedef struct {
    char dummy;
} A;

#define A_foo foo_impl
ML99_EVAL(IFACE99_impl(v(Foo), v(A)));

typedef struct {
    char dummy;
} B;

#define B_foo foo_impl
ML99_EVAL(IFACE99_implExtern(v(Foo), v(B)));
// } (Implementations)

int main(void) {
    // Ensure `impl`-generated data.
    {
        assert(VTABLE(A, Foo).foo == foo_impl);
        assert(VTABLE(B, Foo).foo == foo_impl);
    }

    ENSURE_DYN_OBJ(A, Foo);
    ENSURE_DYN_OBJ(B, Foo);

    return 0;
}
