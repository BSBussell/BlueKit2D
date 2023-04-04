cmake_minimum_required(VERSION 3.14)

project(BlueKit2DProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the library targets
add_subdirectory(BML)
add_subdirectory(BlueKit2D)

# Define the executable target
add_subdirectory(TestGame)