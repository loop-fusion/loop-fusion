#!/usr/bin/env bash

set -euo pipefail
IFS=$'\n\t'

entry="include/loop_fusion/loop_fusion.hpp"

echo "#ifndef LOOP_FUSION_HPP"
echo "#define LOOP_FUSION_HPP"
echo ""
pcpp --passthru-unfound-includes -I include/ $entry
echo ""
echo "#endif"
