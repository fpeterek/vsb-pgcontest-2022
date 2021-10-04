#!/bin/bash

BUILD_TYPE=${1:-Release}

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd "${DIR}" || exit 1

mkdir -p build
cd build || exit 1

echo "Compiling in ${BUILD_TYPE}" mode
cmake -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" ..
make -j4
