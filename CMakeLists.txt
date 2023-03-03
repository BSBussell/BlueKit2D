cmake_minimum_required(VERSION 3.14)

# Project's Name
project(BlueKit2D)

# Set the output folder where The program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_BUILD_TYPE Debug)


# The following folder will be included

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lSDL2")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lSDL2_image")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lSDL2_mixer")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lBML")


add_executable(BlueKit2D)

target_compile_options(
	BlueKit2D
	PRIVATE
	-Wall
	-Wextra
	-std=c++17
	-O3)

target_sources(
	BlueKit2D
	PRIVATE

	# Main
	src/main.cpp
	src/main.h

	# Core Files
	src/Core/BlueEnt.h
	src/Core/BlueTypes.h
	src/Core/BlueCompArray.h
	src/Core/BlueSys.h

	# Components
	src/Components/Transform.h
)

target_include_directories(
	BlueKit2D
	PRIVATE
	src)

#find_package(BML REQUIRED)
#include_directories(${BML_INCLUDE_DIRS})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

target_link_libraries(
	BlueKit2D
	PRIVATE
	${SDL2_LIBRARIES}
	-lSDL2_image
	-lSDL2_mixer
	BML)