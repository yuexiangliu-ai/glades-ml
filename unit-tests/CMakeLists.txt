cmake_minimum_required(VERSION 3.5.1)
project(Tests)

# Compiler Flags (inherit from parent if needed)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-variable -Wno-unused-parameter")

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/Backend/Machine Learning/Networks)
include_directories(/home/ubuntu/shmea/Backend/Database)

# Find GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Test executable
add_executable(tests "Backend/Machine Learning/bayes-test.cpp")

# Link libraries
target_link_libraries(tests
    glades
    ${GTEST_LIBRARIES}
    ${GTEST_MAIN_LIBRARIES}
    pthread
    shmea
)

# Explicitly add test
add_test(NAME BayesTest.NtuplePrediction COMMAND "${CMAKE_CURRENT_BINARY_DIR}/tests")
