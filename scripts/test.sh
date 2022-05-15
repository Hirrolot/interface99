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
    run_test "decl_impl"
    run_test "default_impl"
    run_test "metalang99_compliant"
    run_test "superinterfaces"
    run_test "vcalls"
    run_test "version"
    run_test "extern_impl/test"
fi
