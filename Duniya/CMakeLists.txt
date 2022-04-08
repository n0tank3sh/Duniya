
set(MATH_UTILS
	"src/Math/Mat.hpp"
    "src/Math/Mat.cpp"
    "src/Math/Vect2.hpp"
    "src/Math/Vect2.cpp"
    "src/Math/Vect3.hpp"
    "src/Math/Vect3.cpp"
    "src/Math/Vect4.hpp"
    "src/Math/Vect4.cpp"
    "src/Math/MathUtils.hpp"
)

set(ECS_SRC
    "src/ECS/ECS.hpp"
    "src/ECS/ECS.cpp"
	"src/ECS/CommonComponent.hpp"
    "src/ECS/GraphicsComponent.hpp"
	"src/ECS/SerializerSystem.hpp"
	"src/ECS/SerializerSystem.cpp"
	"src/ECS/Logger.hpp"
	"src/ECS/Logger.cpp"
	)

set(EXCEPTION_SRC
	"src/Exception.cpp"
    "src/Exception.hpp"
	)


set(TEST_GAME_SRC
	"src/TestGame.hpp"
	"src/TestGame.cpp"
	)
set(DUNIYA_SRC
    "src/Main.cpp"
    "src/Application.hpp"
    "src/Application.cpp"
    "src/AssetLoader.hpp"
    "src/AssetLoader.cpp"
	"src/RendererSystem.hpp"
    "src/RendererSystem.cpp"
    "src/Graphics/Renderer.hpp"
	"src/Graphics/Renderer.cpp"
    "src/Graphics/OpenGL/GLUtils.hpp"
    "src/Graphics/OpenGL/GLRenderer.hpp"
    "src/Graphics/OpenGL/GLRenderer.cpp"
	"src/SDLUtiliy.hpp"
	"src/Renderer2DSystem.hpp"
	"src/Renderer2DSystem.cpp"
	"src/TexturePacker.hpp"
	"src/TexturePacker.cpp"
)

set(SCENECONVERTER_SRC
	"src/SceneConverter/SceneConverter.hpp"
	"src/SceneConverter/SceneConverter.cpp"
	"src/ECS/ECS.hpp"
	"src/ECS/ECS.cpp"
	)


include_directories(${SDL2_INCLUDE_DIRS})
add_executable(
	Duniya
	${DUNIYA_SRC}
	${TEST_GAME_SRC}
	${MATH_UTILS}
	${ECS_SRC}
	${EXCEPTION_SRC}
)


target_include_directories(
	Duniya
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)
#target_include_directories(
#	sceneconverter
#	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
#)

set(RESOURCE_TEST_FILE "Resource/Test/model/Earth 2K.obj")

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Resource/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Resource/)

add_custom_target( resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resource ${CMAKE_CURRENT_BINARY_DIR}/Resource
)

add_dependencies(Duniya resources)

target_include_directories(
	Duniya PUBLIC
	${FREETYPE_INCLUDE_DIRS}
)

target_link_libraries(
	Duniya
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    glad
	${OPENGL_LIBRARIES}
	${FREETYPE_LIBRARIES}
    ${CMAKE_DL_LIBS}
)


