#!/usr/bin/env bash

set -euo pipefail
IFS=$'\n\t'

cd /opt/compiler-explorer

# See https://github.com/mattgodbolt/compiler-explorer/blob/master/docs/AddingALibrary.md
cp /opt/c++.local.properties /opt/compiler-explorer/etc/config/
