# Loop Fusion

[![CircleCI](https://circleci.com/gh/loop-fusion/loop-fusion.svg?style=svg)](https://circleci.com/gh/loop-fusion/loop-fusion)

`loop_fusion` is a header-only library for merging multiple loop bodies of equal or different ranges for more efficient execution that can benefit from CPU caching.

For more detailed information, please refer to the [documentation](docs/) and the [presentation slides](docs/LoopFusionPresentation.pdf).

## Goal
We try to create a header-only library that allows users to manually
fuse/merge loops.

### What is loop fusion?
Assume you have the following code:

```cpp
for (size_t i = 0; i < a.size(); ++i) {
    a[i] = b[i] + c[i];
}
// [...] code that is independent from the loop
for (size_t i = 0; i < a.size(); ++i) {
    d[i] = a[i] + f[i];
}
```

We see that both loops are independent. The compiler *could* fuse those two loops
to a single one to save one pass. However it most likely will not because there
*may* be side effects that the compiler is not able to detect.

By using this library, users can merge the loops above by wrapping their contents
in lambdas like the following examples shows:

```cpp
#include <loop_fusion/loop_fusion.hpp>
int main(int argc, char** argv) {
    using namespace loop_fusion::main_range;

    // a, b, c, d and f are arrays declared outside of this example
    auto loop_body_1 = [&](size_t i) { a[i] = b[i] + c[i]; };
    auto loop_body_2 = [&](size_t i) { d[i] = a[i] + f[i]; };

    // The following line merges and executes the two loop bodies
    // so that they are executed after another in just one loop.
    (loop_to(a.size()) | loop_body_1 | loop_body_2).run();

    return 0;
}
```


## Compiler Support
We require a C++17 compliant compiler. Our CI checks this library against the
latest Clang and GCC versions. We therefore support:

 - GCC 9 and higher
 - Clang 9 and higher
 - MSVC 19.24 and higher

Other compilers and versions may work but are not tested against.


## Third Party Libraries
This project uses the following libraries for testing only, i.e. no runtime
dependencies besides the standard library are required:

 - Catch2
   - Website: https://github.com/catchorg/Catch2
   - License: [Boost Software License 1.0](https://github.com/catchorg/Catch2/blob/master/LICENSE.txt)
   - Reason: Testing framework

We also use some third-party CMake modules. See [`cmake/README.md`](./cmake/README.md).
