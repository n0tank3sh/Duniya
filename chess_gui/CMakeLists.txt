

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

add_executable(
    chess_gui
    ${CHESS_SRC}
)
