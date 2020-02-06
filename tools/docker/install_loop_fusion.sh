#!/usr/bin/env bash

set -euo pipefail
IFS=$'\n\t'

cd /opt

git clone https://github.com/loop-fusion/loop-fusion.git
cd loop_fusion
git checkout master

mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DFUSION_BUILD_TEST=OFF
cmake --build . --target install
