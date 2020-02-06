#!/usr/bin/env bash

set -euo pipefail
IFS=$'\n\t'

cd /opt
git clone https://github.com/mattgodbolt/compiler-explorer.git

cd compiler-explorer
npm install
