cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)

# locate gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# create unit test executable
add_executable(hash_table_test hash_table_tests.cpp)
target_link_libraries(hw5test ${GTEST_LIBRARIES} pthread)

# create performance executable
add_executable(hash_table_perf hash_table_perf.cpp)
