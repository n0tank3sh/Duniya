
find_path(ASSIMP_INCLUDE_DIR assimp/mesh.h
	/usr/include
	/usr/local/include
	/opt/local/include
)
find_library(ASSIMP_LIBRARY assimp
	/usr/lib64
	/usr/lib
	/usr/local/lib
	/opt/local/lib
)

if(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY)
	set(ASSIMP_FOUND TRUE)
	set(ASSIMPL_LIBRARIES ${ASSIMP_LIBRARY}
)
endif()

