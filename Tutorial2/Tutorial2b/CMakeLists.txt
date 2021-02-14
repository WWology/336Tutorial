configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SimpleVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SimpleFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader2b
	shader.cpp
	shader.h
)

add_executable(Tutorial2b Tutorial2b.cpp)

target_link_libraries(Tutorial2b PUBLIC
	GraphicsSDK
	Shader2b
)

pvs_studio_add_target(TARGET Tutorial2b.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial2b Shader2b
	MODE GA:1,2 OP
	LOG Tutorial2b.err
	HIDE_HELP
)