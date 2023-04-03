cmake_minimum_required(VERSION 3.14)

project(TestGame)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TestGame)

target_compile_options(
		TestGame
		PRIVATE
		-Wall
		-Wextra
		-std=c++17
		#-Ofast
)

target_sources(
		TestGame
		PRIVATE

		# Main
		main.cpp
		main.h

		# Scenes
		Scenes/SimpleScene.h
		Scenes/StressScene.h
)

target_include_directories(
		TestGame
		PRIVATE
		/usr/local/include/
		.
)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

target_link_libraries(
		TestGame
		PRIVATE

		# Required SDL2 libraries
		${SDL2_LIBRARIES}
		-lSDL2_image
		-lSDL2_ttf
		-lSDL2_mixer

		# Required BlueKit libraries
		-lBML
		-lBlueKit2D
)

set_target_properties(TestGame PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../user/
		)

# Output the runtime output directory
message(STATUS "Runtime output directory: ${CMAKE_CURRENT_BINARY_DIR}/../../user/")