cmake_minimum_required(VERSION 3.15)
project("chess gui")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_subdirectory(chess_gui)
add_subdirectory(glad)
