

set(CHESS_SRC

"src/Application.cpp"
"src/ecs.h"
"src/ecs.cpp"
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
include_directories(${SDL2_INCLUDE_DIRS}/SDL2)
add_executable(
    chess_gui
    ${CHESS_SRC}
)

target_include_directories(
    chess_gui
    PRIVATE ${CHESS_SRC}
)

target_link_libraries(
    chess_gui
    ${SDL2_LIBRARIES}
)


