
set(MATH_UTILS
	"src/Math/Mat.h"
    "src/Math/Mat.cpp"
    "src/Math/Vect2.h"
    "src/Math/Vect2.cpp"
    "src/Math/Vect3.h"
    "src/Math/Vect3.cpp"
    "src/Math/Vect4.h"
    "src/Math/Vect4.cpp"
    "src/Math/MathUtils.h"
)

set(ECS_SRC
    "src/ECS/ECS.h"
    "src/ECS/ECS.cpp"
	"src/ECS/CommonComponent.h"
    "src/ECS/GraphicsComponent.h"
	"src/ECS/SerializerSystem.h"
	"src/ECS/SerializerSystem.cpp"
	"src/ECS/Logger.h"
	"src/ECS/Logger.cpp"
	)

set(EXCEPTION_SRC
	"src/Exception.cpp"
    "src/Exception.h"
	)


set(TEST_GAME_SRC
	"src/TestGame.h"
	"src/TestGame.cpp"
	)
set(CHESS_SRC
    "src/Main.cpp"
    "src/Application.h"
    "src/Application.cpp"
    "src/AssetLoader.h"
    "src/AssetLoader.cpp"
	"src/RendererSystem.h"
    "src/RendererSystem.cpp"
    "src/Graphics/Renderer.h"
	"src/Graphics/Renderer.cpp"
    "src/Graphics/OpenGL/GLUtils.h"
    "src/Graphics/OpenGL/GLRenderer.h"
    "src/Graphics/OpenGL/GLRenderer.cpp"
	"src/SDLUtiliy.h"
	"src/Renderer2DSystem.h"
	"src/Renderer2DSystem.cpp"
	"src/TexturePacker.h"
	"src/TexturePacker.cpp"
)

set(SCENECONVERTER_SRC
	"src/SceneConverter/SceneConverter.h"
	"src/SceneConverter/SceneConverter.cpp"
	"src/ECS/ECS.h"
	"src/ECS/ECS.cpp"
	)


include_directories(${SDL2_INCLUDE_DIRS})
add_executable(
    chess_gui
    ${CHESS_SRC}
	${TEST_GAME_SRC}
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

set(RESOURCE_TEST_FILE "Resource/Test/model/Earth 2K.obj")
set(TEST_FILE "justChecking.dick")

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Resource/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Resource/)

add_custom_target( resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resource ${CMAKE_CURRENT_BINARY_DIR}/Resource
)
add_custom_target( checking_chess_gui
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/sceneconverter ${RESOURCE_TEST_FILE} ${TEST_FILE}
)
#add_dependencies(chess_gui resources checking_chess_gui)
#add_dependencies(checking_chess_gui resources sceneconverter)

add_dependencies(chess_gui resources)

target_include_directories(
	chess_gui PUBLIC
	${FREETYPE_INCLUDE_DIRS}
)

target_link_libraries(
    chess_gui
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    glad
	${OPENGL_LIBRARIES}
	${FREETYPE_LIBRARIES}
    ${CMAKE_DL_LIBS}
)


target_link_libraries(
	sceneconverter
	${ASSIMP_LIBRARIES}
)

