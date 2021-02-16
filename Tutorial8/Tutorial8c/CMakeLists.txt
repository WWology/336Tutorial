add_subdirectory(images)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/LightAndTextureVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ScreenQuadVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/LightAndTextureFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ScreenQuadFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(bmpfuncs8c
	bmpfuncs.h
	bmpfuncs.cpp
)

add_library(Shader8c
	shader.h
	shader.cpp
)

add_executable(Tutorial8c Tutorial8c.cpp)

target_link_libraries(Tutorial8c PUBLIC
	GraphicsSDK
	Shader8c
	bmpfuncs8c
)

pvs_studio_add_target(TARGET Tutorial8c.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial8c Shader8c bmpfuncs8c
	MODE GA:1,2 OP
	LOG Tutorial8c.err
	HIDE_HELP
)