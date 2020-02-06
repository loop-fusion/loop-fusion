# Loop Fusion Examples

These examples demonstrate how to use `loop_fusion`.
They also show how to use it with CMake. They expect `loop_fusion` to be
installed on your system.

If you want to build the examples as part of this project, you can run:

```sh
# Build & Install loop_fusion
cd loop_fusion # git project
mkdir build && cd build
# Disable tests and (optionally) set the installation directory.
INSTALL_PATH=$(pwd)/install_prefix
cmake .. -DFUSION_BUILD_TEST=OFF -DCMAKE_INSTALL_PREFIX=${INSTALL_PATH}
# Install to the directory set before
cmake --build . --config Release --target install

# Now build one example project (e.g. compiletime_basic).
# Setting the prefix path is only required, if it
# was customized like above.
cd ../examples/compiletime_basic
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=${INSTALL_PATH}
cmake --build . --config Release
```
