add_subdirectory(images)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/LightAndTextureVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/LightAndTextureFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader6a
	shader.h
	shader.cpp
)

add_library(bmpfuncs6a
	bmpfuncs.h
	bmpfuncs.cpp
)

add_executable(Tutorial6a Tutorial6a.cpp)

target_link_libraries(Tutorial6a PUBLIC
	GraphicsSDK
	Shader6a
	bmpfuncs6a
)

pvs_studio_add_target(TARGET Tutorial6a.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial6a Shader6a bmpfuncs6a
	MODE GA:1,2 OP
	LOG Tutorial6a.err
	HIDE_HELP
)