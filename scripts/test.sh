#!/bin/bash

mkdir -p tests/build
cd tests/build
cmake ..
cmake --build .

run_test() {
    echo "executing ./$1 ..."
    ./$1
}

if [[ "$OSTYPE" == "linux-gnu" ]]; then
    run_test "basic_tests"
    run_test "vcalls"
    run_test "decl_impl"
    run_test "forward_typedefs"
    run_test "impl_primary"
    run_test "metalang99_compliant"
    run_test "superinterfaces"
    run_test "extern_impl/test"
fi
