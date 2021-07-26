cmake_minimum_required(VERSION 3.15)
project("chess gui")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

INCLUDE(FindPkgConfig)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ASSIMP REQUIRED)

add_subdirectory(chess_gui)
add_subdirectory(glad)
