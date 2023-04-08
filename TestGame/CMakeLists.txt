cmake_minimum_required(VERSION 3.14)

project(Game)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Game)

target_compile_options(
		Game
		PRIVATE
		-Wall
		-Wextra
		-std=c++17
		#-Ofast
)

target_sources(
		Game
		PRIVATE

		# Main
		main.cpp
		main.h

		# Scenes
		Scenes/SimpleScene.h
		Scenes/StressScene.h
)

target_include_directories(
		Game
		PRIVATE
		/usr/local/include/
		.
)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

target_link_libraries(
		Game
		PRIVATE

		# Required SDL2 libraries
		${SDL2_LIBRARIES}
		-lSDL2_image
		-lSDL2_ttf
		-lSDL2_mixer

		# Required BlueKit libraries
		-lBlueKit2D
		-lBML
)

# We put the executable in the same directory as the source files, that way you don't have to do
# ../resources/... to access the resources
set_target_properties(Game PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# Output the runtime output directory
message(STATUS "Game Runtime output directory: ${CMAKE_CURRENT_SOURCE_DIR}")