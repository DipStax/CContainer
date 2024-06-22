cmake_minimum_required(VERSION 3.22.1)

project(C-Container LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)

add_compile_options(-Wall -Wextra -Werror -g3)

file(GLOB_RECURSE SRC_CC
    src/**.c
)

add_library(CContainer SHARED ${SRC_CC})

set_target_properties(CContainer PROPERTIES
    C_STANDARD 17
    C_EXTENSIONS ON
)

target_link_libraries(CContainer gcov)

target_include_directories(CContainer PRIVATE include)

include(FetchContent)

set(GTEST_VERSION 1.14.0)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v${GTEST_VERSION}.tar.gz
    PREFIX ${CMAKE_BINARY_DIR}/gtest
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/gtest
)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE SRC_TESTS
    test/**.cpp
)

add_executable(ContainerTest
    ${SRC_TESTS}
)

target_include_directories(ContainerTest PRIVATE include)
target_include_directories(ContainerTest PRIVATE test)
target_link_libraries(ContainerTest CContainer GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(ContainerTest)