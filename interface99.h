/*
MIT License

Copyright (c) 2021-2023 Hirrolot

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// The official repository: <https://github.com/Hirrolot/interface99>.

#ifndef INTERFACE99_H
#define INTERFACE99_H

#include <metalang99.h>

#if !ML99_VERSION_COMPATIBLE(1, 13, 2)
#error Please, update Metalang99 to v1.13.2 or later.
#endif

#ifndef IFACE99_NO_ALIASES

#define interface(iface)                   interface99(iface)
#define impl(iface, implementer)           impl99(iface, implementer)
#define implExtern(iface, implementer)     implExtern99(iface, implementer)
#define declImpl(iface, implementer)       declImpl99(iface, implementer)
#define declImplExtern(iface, implementer) declImplExtern99(iface, implementer)
#define vfunc(ret_ty, name, ...)           vfunc99(ret_ty, name, __VA_ARGS__)
#define vfuncDefault(ret_ty, name, ...)    vfuncDefault99(ret_ty, name, __VA_ARGS__)

#define VCALL(obj, ...)                       VCALL99(obj, __VA_ARGS__)
#define VCALL_OBJ(obj, ...)                   VCALL_OBJ99(obj, __VA_ARGS__)
#define VCALL_SUPER(obj, superiface, ...)     VCALL_SUPER99(obj, superiface, __VA_ARGS__)
#define VCALL_SUPER_OBJ(obj, superiface, ...) VCALL_SUPER_OBJ99(obj, superiface, __VA_ARGS__)

#define DYN(implementer, iface, ...)     DYN99(implementer, iface, __VA_ARGS__)
#define DYN_LIT(implementer, iface, ...) DYN_LIT99(implementer, iface, __VA_ARGS__)
#define VTABLE(implementer, iface)       VTABLE99(implementer, iface)

#define VSelf    VSelf99
#define VSELF(T) VSELF99(T)

#endif // IFACE99_NO_ALIASES

// Public stuff {

// Metalang99-compliant macros {

#define IFACE99_interface(iface)               ML99_call(IFACE99_interface, iface)
#define IFACE99_impl(iface, implementer)       ML99_call(IFACE99_impl, iface, implementer)
#define IFACE99_implExtern(iface, implementer) ML99_call(IFACE99_implExtern, iface, implementer)

#define interface99(iface)               ML99_EVAL(IFACE99_interface_IMPL(iface))
#define impl99(iface, implementer)       ML99_EVAL(IFACE99_impl_IMPL(iface, implementer))
#define implExtern99(iface, implementer) ML99_EVAL(IFACE99_implExtern_IMPL(iface, implementer))
// } (Metalang99-compliant macros)

#define vfunc99(ret_ty, name, ...)        ML99_CHOICE(vfunc, ret_ty, name, __VA_ARGS__)
#define vfuncDefault99(ret_ty, name, ...) ML99_CHOICE(vfuncDefault, ret_ty, name, __VA_ARGS__)

#define DYN99(implementer, iface, ...)                                                             \
    ((iface){.self = (void *)(__VA_ARGS__), .vptr = &VTABLE99(implementer, iface)})
#define DYN_LIT99(implementer, iface, ...) DYN99(implementer, iface, &(implementer)__VA_ARGS__)
#define VTABLE99(implementer, iface)       ML99_CAT4(implementer, _, iface, _impl)

#define VSelf99 void *restrict iface99_self

// clang-format off

#define VSELF99(T) T *restrict self = (T *restrict)(iface99_self)
// clang-format on

#define IFACE99_MAJOR 1
#define IFACE99_MINOR 0
#define IFACE99_PATCH 1

#define IFACE99_VERSION_COMPATIBLE(x, y, z)                                                        \
    (IFACE99_MAJOR == (x) &&                                                                       \
     ((IFACE99_MINOR == (y) && IFACE99_PATCH >= (z)) || (IFACE99_MINOR > (y))))

#define IFACE99_VERSION_EQ(x, y, z)                                                                \
    (IFACE99_MAJOR == (x) && IFACE99_MINOR == (y) && IFACE99_PATCH == (z))
// } (Public stuff)

// Interface generation {

#define IFACE99_interface_IMPL(iface)                                                              \
    ML99_TERMS(                                                                                    \
        v(typedef struct iface##VTable iface##VTable;),                                            \
        v(typedef struct iface iface;),                                                            \
        ML99_semicoloned(ML99_struct(v(iface##VTable), IFACE99_PRIV_genVTableFields(iface))),      \
        v(struct iface {                                                                           \
            void *self;                                                                            \
            const iface##VTable *vptr;                                                             \
        }))

/*
 * // Only if <iface> is a marker interface without superinterfaces:
 * char dummy;
 *
 * <func-ret-ty>0 (*<func-name>0)(<func-params>0);
 * ...
 * <func-ret-ty>N (*<func-name>N)(<func-params>N);
 *
 * const <requirement>0VTable *<requirement>;
 * ...
 * const <requirement>NVTable *<requirement>;
 */
#define IFACE99_PRIV_genVTableFields(iface)                                                        \
    ML99_uncomma(ML99_QUOTE(                                                                       \
        IFACE99_PRIV_genDummy(iface),                                                              \
        ML99_IF(                                                                                   \
            IFACE99_PRIV_IS_MARKER_IFACE(iface),                                                   \
            ML99_empty(),                                                                          \
            IFACE99_PRIV_genFuncPtrForEach(iface)),                                                \
        ML99_IF(                                                                                   \
            IFACE99_PRIV_IS_SUB_IFACE(iface),                                                      \
            IFACE99_PRIV_genRequirementForEach(iface),                                             \
            ML99_empty())))

#define IFACE99_PRIV_genDummy(iface)                                                               \
    ML99_IF(IFACE99_PRIV_IS_EMPTY_VTABLE(iface), v(char dummy;), ML99_empty())

/*
 * <func-ret-ty>0 (*<func-name>0)(<func-params>0);
 * ...
 * <func-ret-ty>N (*<func-name>N)(<func-params>N);
 */
#define IFACE99_PRIV_genFuncPtrForEach(iface)                                                      \
    ML99_seqForEach(v(IFACE99_PRIV_genFuncPtr), v(iface##_IFACE))

#define IFACE99_PRIV_genFuncPtr_IMPL(_tag, ret_ty, name, ...) v(ret_ty (*name)(__VA_ARGS__);)

/*
 * const <requirement>0VTable *<requirement>;
 * ...
 * const <requirement>NVTable *<requirement>;
 */
#define IFACE99_PRIV_genRequirementForEach(iface)                                                  \
    ML99_tupleForEach(v(IFACE99_PRIV_genRequirement), v(iface##_EXTENDS))

#define IFACE99_PRIV_genRequirement_IMPL(requirement) v(const requirement##VTable *requirement;)
// } (Interface generation)

// Interface implementation generation {

#define IFACE99_impl_IMPL(iface, implementer)                                                      \
    IFACE99_PRIV_implCommon(IFACE99_PRIV_STORAGE_CLASS_STATIC, iface, implementer)
#define IFACE99_implExtern_IMPL(iface, implementer)                                                \
    IFACE99_PRIV_implCommon(IFACE99_PRIV_STORAGE_CLASS_EXTERN, iface, implementer)

#define IFACE99_PRIV_STORAGE_CLASS_STATIC    static
#define IFACE99_PRIV_STORAGE_CLASS_EXTERN    /* If no storage-class specifier is provided, the     \
                                                default for objects is `extern` (file scope) or    \
                                                `auto` (block scope). */

#define IFACE99_PRIV_implCommon(storage_class, iface, implementer)                                 \
    ML99_assignInitializerList(                                                                    \
        v(storage_class const iface##VTable VTABLE99(implementer, iface)),                         \
        IFACE99_PRIV_genImplInitList(iface, implementer))

/*
 * // Only if <iface> is a marker interface without superinterfaces:
 * .dummy = '\0',
 *
 * <func-name>0 = either <implementer>_<func-name>0 or <iface>_<func-name>0,
 * ...
 * <func-name>N = either <implementer>_<func-name>N or <iface>_<func-name>N,
 *
 * <requirement>0 = &VTABLE(<implementer>, <requirement>0),
 * ...
 * <requirement>N = &VTABLE(<implementer>, <requirement>N),
 */
#define IFACE99_PRIV_genImplInitList(iface, implementer)                                           \
    ML99_uncomma(ML99_QUOTE(                                                                       \
        IFACE99_PRIV_genDummyInit(iface),                                                          \
        ML99_IF(                                                                                   \
            IFACE99_PRIV_IS_MARKER_IFACE(iface),                                                   \
            ML99_empty(),                                                                          \
            IFACE99_PRIV_genImplFuncNameForEach(iface, implementer)),                              \
        ML99_IF(                                                                                   \
            IFACE99_PRIV_IS_SUB_IFACE(iface),                                                      \
            IFACE99_PRIV_genRequirementsImplForEach(iface, implementer),                           \
            ML99_empty())))

#define IFACE99_PRIV_genDummyInit(iface)                                                           \
    ML99_IF(IFACE99_PRIV_IS_EMPTY_VTABLE(iface), v(.dummy = '\0'), ML99_empty())

/*
 * <func-name>0 = either <implementer>_<func-name>0 or <iface>_<func-name>0,
 * ...
 * <func-name>N = either <implementer>_<func-name>N or <iface>_<func-name>N,
 */
#define IFACE99_PRIV_genImplFuncNameForEach(iface, implementer)                                    \
    ML99_seqForEach(                                                                               \
        ML99_appl(v(IFACE99_PRIV_genImplFuncName), v(iface, implementer)),                         \
        v(iface##_IFACE))

#define IFACE99_PRIV_genImplFuncName_IMPL(iface, implementer, tag, _ret_ty, name, ...)             \
    ML99_match(ML99_choice(v(tag), v(iface, implementer, name)), v(IFACE99_PRIV_genImpl_))

#define IFACE99_PRIV_genImpl_vfunc_IMPL(_iface, implementer, name) v(.name = implementer##_##name, )
#define IFACE99_PRIV_genImpl_vfuncDefault_IMPL(iface, implementer, name)                           \
    ML99_IF(                                                                                       \
        IFACE99_PRIV_IS_CUSTOM(implementer, name),                                                 \
        IFACE99_PRIV_genImpl_vfunc_IMPL(~, implementer, name),                                     \
        v(.name = iface##_##name, ))

/*
 * <requirement>0 = &VTABLE(<implementer>, <requirement>0),
 * ...
 * <requirement>N = &VTABLE(<implementer>, <requirement>N),
 */
#define IFACE99_PRIV_genRequirementsImplForEach(iface, implementer)                                \
    ML99_tupleForEach(                                                                             \
        ML99_appl(v(IFACE99_PRIV_genRequirementImpl), v(implementer)),                             \
        v(iface##_EXTENDS))

#define IFACE99_PRIV_genRequirementImpl_IMPL(implementer, requirement)                             \
    v(.requirement = &VTABLE99(implementer, requirement), )
// } (Interface implementation generation)

// Implementation declaration {

#define declImpl99(iface, implementer) static IFACE99_PRIV_DECL_IMPL_COMMON(iface, implementer)
#define declImplExtern99(iface, implementer)                                                       \
    extern IFACE99_PRIV_DECL_IMPL_COMMON(iface, implementer)

#define IFACE99_PRIV_DECL_IMPL_COMMON(iface, implementer)                                          \
    const ML99_CAT(iface, VTable) VTABLE99(implementer, iface)
// } (Implementation declaration)

// Virtual calls {

#define VCALL99(obj, ...)                                                                          \
    ((obj).vptr->IFACE99_PRIV_VCALL_OVERLOAD(__VA_ARGS__)((obj).self, __VA_ARGS__))
#define VCALL_OBJ99(obj, ...)                                                                      \
    ((obj).vptr->IFACE99_PRIV_VCALL_OVERLOAD(__VA_ARGS__)((obj), __VA_ARGS__))
#define VCALL_SUPER99(obj, superiface, ...)                                                        \
    ((obj).vptr->superiface->IFACE99_PRIV_VCALL_OVERLOAD(__VA_ARGS__)((obj).self, __VA_ARGS__))
#define VCALL_SUPER_OBJ99(obj, superiface, ...)                                                    \
    ((obj).vptr->superiface->IFACE99_PRIV_VCALL_OVERLOAD(__VA_ARGS__)(                             \
        ((superiface){                                                                             \
            .self = (obj).self,                                                                    \
            .vptr = (obj).vptr->superiface,                                                        \
        }),                                                                                        \
        __VA_ARGS__))

#define IFACE99_PRIV_VCALL_OVERLOAD(...)                                                           \
    ML99_CAT(IFACE99_PRIV_VCALL_, ML99_VARIADICS_IS_SINGLE(__VA_ARGS__))
#define IFACE99_PRIV_VCALL_1(obj, func_name)      func_name(obj)
#define IFACE99_PRIV_VCALL_0(obj, func_name, ...) func_name(obj, __VA_ARGS__)
// } (Virtual calls)

// Various predicates {

#define IFACE99_PRIV_IS_EMPTY_VTABLE(iface)                                                        \
    ML99_AND(IFACE99_PRIV_IS_MARKER_IFACE(iface), ML99_NOT(IFACE99_PRIV_IS_SUB_IFACE(iface)))
#define IFACE99_PRIV_IS_MARKER_IFACE(iface) ML99_SEQ_IS_EMPTY(iface##_IFACE)
#define IFACE99_PRIV_IS_SUB_IFACE(iface)    ML99_IS_TUPLE(iface##_EXTENDS)

#define IFACE99_PRIV_IS_CUSTOM(implementer, func_name)                                             \
    ML99_IS_TUPLE(implementer##_##func_name##_CUSTOM)
// } (Various predicates)

// Arity specifiers {

#define IFACE99_PRIV_genRequirement_ARITY     1
#define IFACE99_PRIV_genRequirementImpl_ARITY 2

#define IFACE99_PRIV_genFuncPtr_ARITY      1
#define IFACE99_PRIV_genImplFuncName_ARITY 2

// Public:

#define IFACE99_interface_ARITY  1
#define IFACE99_impl_ARITY       2
#define IFACE99_implExtern_ARITY 2
// } (Arity specifiers)

#endif // INTERFACE99_H
