#!/usr/bin/env bash

set -e

PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$PROJECT_ROOT/builds"
CMAKE_DIR="$BUILD_DIR/cmake"

BUILD_TYPE=${1:-Release}
CLEAN_FLAG=$2

if [ "$CLEAN_FLAG" == "clean" ]; then
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

echo "Ensuring build directory exists..."
mkdir -p "$CMAKE_DIR"

echo "Configuring project ($BUILD_TYPE)..."
cmake -S "$PROJECT_ROOT" -B "$CMAKE_DIR" \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE

echo "Building..."
cmake --build "$CMAKE_DIR" -j$(nproc)

echo "Done."
echo "Binary: $BUILD_DIR/bin"