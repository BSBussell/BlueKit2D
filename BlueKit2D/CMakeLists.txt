cmake_minimum_required(VERSION 3.14)

# Project's Name
project(BlueKit2D)

# Create a variable BlueKit2D_INSTALL and set it to ON
option(BlueKit2D_INSTALL "Install BlueKit2D" ON)

# Set the output folder where The program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/BlueKit2D/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Tell CMake to place the build
set(CMAKE_CURRENT_BINARY_DIR ${PROJECT_BINARY_DIR}/build)
message(STATUS "Current Binary Dir: ${CMAKE_CURRENT_BINARY_DIR}")

# Tell CMake where to place the libraries
set(CMAKE_CURRENT_LIBRARY_DIR ${PROJECT_BINARY_DIR}/build)
message(STATUS "Current Library Dir: ${CMAKE_CURRENT_LIBRARY_DIR}")


set(CMAKE_BUILD_TYPE Debug)

# MAKEFILE TELL US YOUR SECRETS!!!
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(BlueKit2D STATIC src/BlueKit2D.h)

target_compile_options(
	BlueKit2D
	PRIVATE
	-Wall
	-Wextra
	-std=c++17
	#-Ofast
	)

target_sources(
	BlueKit2D
	PRIVATE

	# Game Class
	src/BlueGame.cpp
	src/BlueGame.h

	# Core Files
	src/Core/BlueSys.h
	src/Core/BlueTypes.h
	src/Core/BlueBridge.h
	src/Core/BlueCompArray.h
	src/Core/BlueEntManager.h
	src/Core/BlueSysManager.h
	src/Core/BlueCompManager.h
	src/Core/BlueScene.h
	src/Core/BlueSceneManager.h
	
	# Systems
	src/Systems/SpriteSystem.cpp
	src/Systems/SpriteSystem.h
	src/Systems/PhysicsSystem.cpp
	src/Systems/PhysicsSystem.h

	# Components
	src/Components/Transform.h
	src/Components/Sprite.h
	src/Components/PhysicsObject.h

)

target_include_directories(
	BlueKit2D
	PRIVATE
	src
)


find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

target_link_libraries(
	BlueKit2D
	PRIVATE
	${SDL2_LIBRARIES}
	-lSDL2_image
	-lSDL2_mixer
	-lSDL2_ttf
	-lBML)

set_target_properties(BlueKit2D PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/
)

if(BlueKit2D_INSTALL)

	install(TARGETS BlueKit2D
			ARCHIVE DESTINATION lib
			)

	install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/
			DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/usr/local/include/BlueKit2D
			FILES_MATCHING PATTERN "*.h")
endif()


# Print the output_directory
message(STATUS "BlueKit Runtime Directory: ${CMAKE_CURRENT_SOURCE_DIR}/bin/")