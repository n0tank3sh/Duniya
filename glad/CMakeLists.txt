
set(GLAD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(glad ${GLAD_INCLUDE_DIR} "src/glad.c")
target_include_directories(glad PUBLIC ${GLAD_INCLUDE_DIR})

