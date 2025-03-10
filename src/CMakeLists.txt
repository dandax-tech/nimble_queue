cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)  # Enable C++17
set(CMAKE_BUILD_TYPE Debug)
add_definitions( -DDEBUG )

project(nimble_queue)  # Define the project name

add_executable(atomic_bitset_calculator_tester atomic_bitset_calculator_tester.cpp)  # Create an executable from a source file
add_executable(queue_tester queue_tester.cpp)  # Create an executable from a source file
add_executable(bitset_tester bitset_tester.cpp)  # Create an executable from a source file
