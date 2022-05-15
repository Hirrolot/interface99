#!/bin/bash

set -e

mkdir -p examples/build
cd examples/build
cmake ..
cmake --build .

run_example() {
    echo "executing ./$1 ..."
    ./$1
}

if [[ "$OSTYPE" == "linux-gnu" ]]; then
    run_example "shape"
    run_example "tracing_vehicle"
    run_example "airplane"
    run_example "read_write"
    run_example "read_write_both"
    run_example "marker"
    run_example "default_impl"

    run_example "opaque_type/frog"
fi
