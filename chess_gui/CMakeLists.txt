
set(MATH_UTILS
	"src/math/mat.h"
    "src/math/mat.cpp"
    "src/math/vect2.h"
    "src/math/vect2.cpp"
    "src/math/vect3.h"
    "src/math/vect3.cpp"
    "src/math/vect4.h"
    "src/math/vect4.cpp"
    "src/math/mathutils.h"
)

set(ECS_SRC
    "src/ecs/ecs.h"
    "src/ecs/ecs.cpp"
	"src/ecs/commoncomponent.h"
    "src/ecs/GraphicsComponent.h"
	"src/ecs/SerializerSystem.h"
	"src/ecs/SerializerSystem.cpp"
	)

set(EXCEPTION_SRC
	"src/Exception.cpp"
    "src/Exception.h"
	)

set(CHESS_SRC
    "src/main.cpp"
    "src/Application.h"
    "src/Application.cpp"
    "src/AssetLoader.h"
    "src/AssetLoader.cpp"
	"src/RendererSystem.h"
    "src/RendererSystem.cpp"
    "src/Graphics/Renderer.h"
	"src/Graphics/Renderer.cpp"
    "src/Graphics/opengl/GLUtils.h"
    "src/Graphics/opengl/GLRenderer.h"
    "src/Graphics/opengl/GLRenderer.cpp"
	"src/Renderer2DSystem.h"
	"src/Renderer2DSystem.cpp"
)

set(SCENECONVERTER_SRC
	"src/SceneConverter/SceneConverter.h"
	"src/SceneConverter/SceneConverter.cpp"
	"src/ecs/ecs.h"
	"src/ecs/ecs.cpp"
	)


include_directories(${SDL2_INCLUDE_DIRS})
add_executable(
    chess_gui
    ${CHESS_SRC}
	${MATH_UTILS}
	${ECS_SRC}
	${EXCEPTION_SRC}
)
add_executable(
	sceneconverter
	${SCENECONVERTER_SRC}
	${MATH_UTILS}
	${ECS_SRC}
	${EXCEPTION_SRC}
)

target_include_directories(
    chess_gui
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_include_directories(
	sceneconverter
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(RESOURCE_TEST_FILE "Resource/Test/Goat.obj")
set(TEST_FILE "justChecking.dick")

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Resource/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Resource/)

add_custom_target( resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resource ${CMAKE_CURRENT_BINARY_DIR}/Resource
)
add_custom_target( checking_chess_gui
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/sceneconverter ${RESOURCE_TEST_FILE} ${TEST_FILE}
)
add_dependencies(chess_gui resources checking_chess_gui)
add_dependencies(checking_chess_gui resources sceneconverter)

target_link_libraries(
    chess_gui
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    glad
	${OPENGL_LIBRARIES}
    ${CMAKE_DL_LIBS}
)


target_link_libraries(
	sceneconverter
	${ASSIMP_LIBRARIES}
)

