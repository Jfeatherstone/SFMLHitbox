cmake_minimum_required(VERSION 3.13)

# Compile in c++ 17
set(CMAKE_CXX_STANDARD 17)

project(hitbox)

# Set our include directories
include_directories(include/)

# This imports the sfml libraries
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

set(SOURCES src/Main.cpp src/Hitbox.cpp)

# Sets our main file
add_executable(${PROJECT_NAME} src/Main.cpp src/Hitbox.cpp)

# This allows us to use the sfml libraries and the filesystem library
target_link_libraries(${PROJECT_NAME} sfml-window sfml-graphics sfml-system sfml-audio)
