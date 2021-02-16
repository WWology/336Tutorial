add_subdirectory(images)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/NormalMapVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/NormalMapFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader6c
	shader.h
	shader.cpp
)

add_library(bmpfuncs6c
	bmpfuncs.h
	bmpfuncs.cpp
)

add_library(Camera6c
	Camera.h
	Camera.cpp
)

add_executable(Tutorial6c Tutorial6c.cpp)

target_link_libraries(Tutorial6c PUBLIC
	GraphicsSDK
	Shader6c
	bmpfuncs6c
	Camera6c
)

pvs_studio_add_target(TARGET Tutorial6c.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial6c Shader6c bmpfuncs6c Camera6c
	MODE GA:1,2 OP
	LOG Tutorial6c.err
	HIDE_HELP
)