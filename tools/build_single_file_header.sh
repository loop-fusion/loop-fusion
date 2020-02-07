#!/usr/bin/env bash

set -euo pipefail
IFS=$'\n\t'

entry="include/loop_fusion/loop_fusion.hpp"
target="loop_fusion.hpp"

pcpp -o combined.hpp --passthru-unfound-includes -I include/ $entry

echo "#ifndef LOOP_FUSION_HPP" > $target
echo "#define LOOP_FUSION_HPP" >> $target
echo "" >> $target
cat combined.hpp >> $target
echo "" >> $target
echo "#endif" >> $target

rm combined.hpp
