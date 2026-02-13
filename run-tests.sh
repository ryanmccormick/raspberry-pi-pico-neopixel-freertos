#!/bin/bash
# Build and run unit tests (native host build, no Pico SDK)

set -e
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build-tests"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
cmake "$SCRIPT_DIR/tests" -DCMAKE_BUILD_TYPE=Debug
make
ctest --output-on-failure
