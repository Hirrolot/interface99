cmake_minimum_required(VERSION 3.16)
project(tests LANGUAGES C)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wall -Wextra -pedantic -ftrack-macro-expansion=0
                      -fsanitize=address)
  add_link_options(-fsanitize=address)
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-fmacro-backtrace-limit=1 -fsanitize=address)
  add_link_options(-fsanitize=address)
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  # Enable a standard-conforming C99/C11 preprocessor.
  add_compile_options("/std:c11")
elseif(CMAKE_C_COMPILER_ID STREQUAL "TinyCC")
  add_compile_definitions(ML99_ALLOW_POOR_DIAGNOSTICS)
endif()

add_subdirectory(extern_impl)

add_executable(basic_tests basic_tests.c common.h common.c util.h)
add_executable(vcalls vcalls.c)
add_executable(decl_impl decl_impl.c common.h common.c)
add_executable(metalang99_compliant metalang99_compliant.c)
add_executable(superinterfaces superinterfaces.c common.h common.c util.h)
add_executable(default_impl default_impl.c)
add_executable(version version.c)

add_subdirectory(.. build)

get_property(
  TESTS
  DIRECTORY .
  PROPERTY BUILDSYSTEM_TARGETS)

get_property(
  EXTERN_IMPL_TESTS
  DIRECTORY extern_impl
  PROPERTY BUILDSYSTEM_TARGETS)

foreach(TARGET ${TESTS};${EXTERN_IMPL_TESTS})
  target_link_libraries(${TARGET} interface99)
  set_target_properties(${TARGET} PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED
                                                           ON)
endforeach()
