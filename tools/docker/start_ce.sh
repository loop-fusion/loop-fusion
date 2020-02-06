#!/usr/bin/env bash

set -euo pipefail
IFS=$'\n\t'

cd /opt/compiler-explorer
make run
