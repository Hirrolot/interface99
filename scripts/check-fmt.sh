#!/bin/bash

./run-clang-format/run-clang-format.py \
    --exclude examples/build \
    --exclude tests/build \
    -r interface99.h tests examples
