

set(CHESS_SRC

        "src/main.cpp"
        "src/Application.h"
        "src/Application.cpp"
        "src/ecs.h"
        "src/ecs.cpp"
        "src/Exception.h"
        "src/RendererSystem.h"
        "src/RendererSystem.cpp"
        "src/math/mat.h"
        "src/math/vect2.h"
        "src/math/vect2.cpp"
        "src/math/vect3.h"
        "src/math/vect4.h"
        "src/math/mathutils.h"
        "src/Graphics/Renderer.h"
        "src/Graphics/opengl/GLUtils.h"
        "src/Graphics/opengl/GLRenderer.h"
        "src/Graphics/opengl/GLRenderer.cpp"
)

find_package(SDL2 REQUIRED)
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


target_link_libraries(
    chess_gui
    ${SDL2_LIBRARIES}
    glad
    OpenGL::GL
)


