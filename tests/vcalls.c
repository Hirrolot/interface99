#include <interface99.h>

#include <assert.h>

#define METHODS(prefix, T)                                                                         \
    vfunc(int, prefix##_vcall, VSelf)                                                              \
    vfunc(int, prefix##_vcall_args, const VSelf, int x, const char *y)                             \
                                                                                                   \
    vfunc(int, prefix##_vcall_obj, T self)                                                         \
    vfunc(int, prefix##_vcall_obj_args, T self, int x, const char *y)

#define TestSuper_IFACE METHODS(test_super, TestSuper)
#define Test_IFACE      METHODS(test, Test)
#define Test_EXTENDS    (TestSuper)

interface(TestSuper);
interface(Test);

static const int n = 7;
static const char *str = "abc";

typedef enum {
    TEST_VCALL,
    TEST_VCALL_ARGS,
    TEST_VCALL_OBJ,
    TEST_VCALL_OBJ_ARGS,
} RetVal;

typedef struct {
    char dummy;
} Impl;

static Impl obj = {0};

#define IMPL_METHODS(prefix, T)                                                                    \
    static int Impl_##prefix##_vcall(VSelf) {                                                      \
        VSELF(Impl);                                                                               \
        assert(&obj == self);                                                                      \
                                                                                                   \
        return TEST_VCALL;                                                                         \
    }                                                                                              \
                                                                                                   \
    static int Impl_##prefix##_vcall_args(const VSelf, int x, const char *y) {                     \
        VSELF(const Impl);                                                                         \
        assert(&obj == self);                                                                      \
        assert(n == x);                                                                            \
        assert(str == y);                                                                          \
                                                                                                   \
        return TEST_VCALL_ARGS;                                                                    \
    }                                                                                              \
                                                                                                   \
    static int Impl_##prefix##_vcall_obj(T test) {                                                 \
        assert(&obj == test.self);                                                                 \
        assert(&VTABLE(Impl, T) == test.vptr);                                                     \
                                                                                                   \
        return TEST_VCALL_OBJ;                                                                     \
    }                                                                                              \
                                                                                                   \
    static int Impl_##prefix##_vcall_obj_args(T test, int x, const char *y) {                      \
        assert(&obj == test.self);                                                                 \
        assert(&VTABLE(Impl, T) == test.vptr);                                                     \
        assert(n == x);                                                                            \
        assert(str == y);                                                                          \
                                                                                                   \
        return TEST_VCALL_OBJ_ARGS;                                                                \
    }

declImpl(TestSuper, Impl);
declImpl(Test, Impl);

// clang-format off
IMPL_METHODS(test_super, TestSuper)
IMPL_METHODS(test, Test)
// clang-format on

impl(TestSuper, Impl);
impl(Test, Impl);

int main(void) {
#define RVALUE DYN(Impl, Test, &obj)

    // Test that VCALL_* can accept lvalues.
    {
        Test lvalue = RVALUE;

        assert(TEST_VCALL == VCALL(lvalue, test_vcall));
        assert(TEST_VCALL_ARGS == VCALL(lvalue, test_vcall_args, n, str));
        assert(TEST_VCALL_OBJ == VCALL_OBJ(lvalue, test_vcall_obj));
        assert(TEST_VCALL_OBJ_ARGS == VCALL_OBJ(lvalue, test_vcall_obj_args, n, str));

        assert(TEST_VCALL == VCALL_SUPER(lvalue, TestSuper, test_super_vcall));
        assert(TEST_VCALL_ARGS == VCALL_SUPER(lvalue, TestSuper, test_super_vcall_args, n, str));
        assert(TEST_VCALL_OBJ == VCALL_SUPER_OBJ(lvalue, TestSuper, test_super_vcall_obj));
        assert(
            TEST_VCALL_OBJ_ARGS ==
            VCALL_SUPER_OBJ(lvalue, TestSuper, test_super_vcall_obj_args, n, str));
    }

    // Test rvalues.
    {
        (void)VCALL(RVALUE, test_vcall);
        (void)VCALL_OBJ(RVALUE, test_vcall_obj);
        (void)VCALL_SUPER(RVALUE, TestSuper, test_super_vcall);
        (void)VCALL_SUPER_OBJ(RVALUE, TestSuper, test_super_vcall_obj);
    }

#undef RVALUE

    return 0;
}
