configure_file(${CMAKE_CURRENT_SOURCE_DIR}/AlphaBlendingVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/AlphaBlendingFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader7b
	shader.h
	shader.cpp
)

add_executable(Tutorial7b Tutorial7b.cpp)

target_link_libraries(Tutorial7b PUBLIC
	GraphicsSDK
	Shader7b
)

pvs_studio_add_target(TARGET Tutorial7b.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial7b Shader7b
	MODE GA:1,2 OP
	LOG Tutorial7b.err
	HIDE_HELP
)