#!/usr/bin/env bash

set -euo pipefail
IFS=$'\n\t'

# This script builds all of our examples.
# It is intended for GitLabCI

SCRIPT_PATH="$( cd "$(dirname "$0")" ; pwd -P )"

cd "${SCRIPT_PATH}/.."
source tools/utils.sh

print_important "Test installation target and examples"

rm -rf build
mkdir build && cd build

INSTALL_PREFIX="$(pwd)/install_prefix"
cmake -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" \
    -DFUSION_BUILD_TEST=OFF \
    ..

cmake --build . --config Release --target install

print_info "Installed Loop Fusion"

cd ../examples
rm -rf build
mkdir build && cd build
BUILD_DIR=$(pwd);
for dir in $(/bin/ls ..); do
    if [[ -d "${BUILD_DIR}/../$dir" ]] && [[ $dir != "build" ]]; then
        cd "$BUILD_DIR"
        echo ""
        echo ""
        print_info "Building example '${dir}' (path is $(pwd))"
        mkdir "${dir}" && cd "${dir}"
        cmake "${BUILD_DIR}/../${dir}" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH="${INSTALL_PREFIX}"
        cmake --build . --config Release
    fi;
done
