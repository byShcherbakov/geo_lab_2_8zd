#!/bin/bash
set -e
cd "$(dirname "$0")"
cmake -S . -B build-macos -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH="$(brew --prefix)"
cmake --build build-macos
./build-macos/CompGeomGraphLab2
