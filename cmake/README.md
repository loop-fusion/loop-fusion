# CMake Modules

This file lists all CMake modules and third party licenses that are used in this project.

`MiscFunctions.cmake`  
 - custom implementation (`loop_fusion`)

 `FindASan.cmake`  
 `FindMSan.cmake`  
 `FindSanitizers.cmake`  
 `FindTSan.cmake`  
 `FindUBSan.cmake`  
 `sanitize-helpers.cmake`  
  - *used only for testing and CI*
  - Website: https://github.com/arsenm/sanitizers-cmake (2018-12-15)
  - License: [MIT License](https://github.com/arsenm/sanitizers-cmake/blob/master/LICENSE)


`coverage.cmake`  
`Gcov.cmake`  
`Findgcov.cmake`  
`Findlcov.cmake`  
  - *used only for testing and CI*
  - Website: https://github.com/cginternals/cmake-init
  - License: [MIT License](https://github.com/cginternals/cmake-init/blob/master/LICENSE)


`Catch.cmake`  
`CatchAddTests.cmake`  
  - *used only for tests*
  - Website: https://github.com/catchorg/Catch2 (2019-01-30)
  - License: [Boost Software License 1.0](https://github.com/catchorg/Catch2/blob/master/LICENSE.txt)
