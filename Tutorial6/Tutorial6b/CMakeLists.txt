add_subdirectory(images)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/MultiTexVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/MultiTexFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader6b
	shader.h
	shader.cpp
)

add_library(bmpfuncs6b
	bmpfuncs.h
	bmpfuncs.cpp
)

add_executable(Tutorial6b Tutorial6b.cpp)

target_link_libraries(Tutorial6b PUBLIC
	GraphicsSDK
	Shader6b
	bmpfuncs6b
)

pvs_studio_add_target(TARGET Tutorial6b.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial6b Shader6b bmpfuncs6b
	MODE GA:1,2 OP
	LOG Tutorial6b.err
	HIDE_HELP
)