cmake_minimum_required(VERSION 3.20)

project(miniad LANGUAGES CXX)

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fetch Catch2
include(cmake/FetchCatch2.cmake)

# Interface library (header-only)
add_library(miniad INTERFACE)

target_include_directories(miniad INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Enable testing and add trivial test target
include(CTest)

enable_testing()

add_executable(tests_dummy tests/test_dummy.cpp)

target_link_libraries(tests_dummy PRIVATE Catch2::Catch2WithMain miniad)

add_test(NAME miniad_dummy COMMAND tests_dummy) 