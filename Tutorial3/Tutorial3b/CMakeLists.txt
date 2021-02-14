configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ModelSpaceVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ColorFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader3b
	shader.h
	shader.cpp
)

add_executable(Tutorial3b Tutorial3b.cpp)

target_link_libraries(Tutorial3b PUBLIC
	GraphicsSDK
	Shader3b
)

pvs_studio_add_target(TARGET Tutorial3b.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial3b Shader3b
	MODE GA:1,2 OP
	LOG Tutorial3b.err
	HIDE_HELP
)