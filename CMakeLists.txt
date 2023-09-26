cmake_minimum_required(VERSION 3.14)

project(BlueKit2DProject LANGUAGES CXX)

# Using locally compiled version of SDL2 because Zorin is lame
#set(SDL2_DIR "/usr/local/lib/cmake/SDL2")


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Install include directory
#set(INSTALL_INCLUDE_DIR usr/local/include)

# Define the library targets
add_subdirectory(BML)
add_subdirectory(BlueKit2D)

# Define the executable target
add_subdirectory(TestGame)



