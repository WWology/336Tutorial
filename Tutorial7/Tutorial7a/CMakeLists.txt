add_subdirectory(images)
add_subdirectory(models)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CubeEnvMapVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CubeEnvMapFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader7a
	shader.h
	shader.cpp
)

add_library(bmpfuncs7a
	bmpfuncs.h
	bmpfuncs.cpp
)

add_library(Camera7a
	Camera.h
	Camera.cpp
)

add_executable(Tutorial7a Tutorial7a.cpp)

target_link_libraries(Tutorial7a PUBLIC
	GraphicsSDK
	Shader7a
	bmpfuncs7a
	Camera7a
)

pvs_studio_add_target(TARGET Tutorial7a.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial7a Shader7a bmpfuncs7a Camera7a
	MODE GA:1,2 OP
	LOG Tutorial7a.err
	HIDE_HELP
)