

set(CHESS_SRC

        "src/main.cpp"
        "src/Application.h"
        "src/Application.cpp"
        "src/AssetLoader.h"
        "src/AssetLoader.cpp"
        "src/ecs.h"
        "src/ecs.cpp"
        "src/Exception.cpp"
        "src/Exception.h"
        "src/RendererSystem.h"
        "src/RendererSystem.cpp"
        "src/math/mat.h"
        "src/math/mat.cpp"
        "src/math/vect2.h"
        "src/math/vect2.cpp"
        "src/math/vect3.h"
        "src/math/vect3.cpp"
        "src/math/vect4.h"
        "src/math/vect4.cpp"
        "src/math/mathutils.h"
        "src/Graphics/Renderer.h"
        "src/Graphics/GraphicsComponent.h"
        "src/Graphics/opengl/GLUtils.h"
        "src/Graphics/opengl/GLRenderer.h"
        "src/Graphics/opengl/GLRenderer.cpp"
)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
add_executable(
    chess_gui
    ${CHESS_SRC}
)

target_include_directories(
    chess_gui
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Resource/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Resource/)

add_custom_target( resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resource ${CMAKE_CURRENT_BINARY_DIR}/Resource
)
add_dependencies(chess_gui resources)

target_link_libraries(
    chess_gui
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    glad
    OpenGL::GL
    ${CMAKE_DL_LIBS}
)


