# Code Quality Tools

We use multiple tools to ensure good code quality of this project.
These tools include:

 - [Cppcheck](http://cppcheck.sourceforge.net/)
 - [`clang-tidy`](https://clang.llvm.org/extra/clang-tidy/)
 - [**Compiler Sanitizers**](https://github.com/google/sanitizers)
   - **Address Sanitizer**
   - **Undefined Behavior Sanitizer**
 - [`shellcheck`](https://www.shellcheck.net/)
 - [`clang-format`](https://clang.llvm.org/docs/ClangFormat.html)
 - [`cmake-format`](https://github.com/cheshirekow/cmake_format)

Please refer to [`CONTRIBUTING.md`](../CONTRIBUTING.md) for further
instructions on how to use these tools.

# A note about code coverage

Due to the nature of this project, code coverage is difficult to measure.
A lot of code is `constexpr` and may be executed during compile time.
