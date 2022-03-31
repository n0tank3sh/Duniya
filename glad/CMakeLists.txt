set(GLAD_SRC src/glad.c)
add_library(glad STATIC ${GLAD_SRC})
target_include_directories(glad PUBLIC ./include)
