add_subdirectory(models)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PerVertexLightingVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ColorFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader5b
	shader.h
	shader.cpp
)

add_executable(Tutorial5b Tutorial5b.cpp)

target_link_libraries(Tutorial5b PUBLIC
	GraphicsSDK
	Shader5b
)

pvs_studio_add_target(TARGET Tutorial5b.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial5b Shader5b
	MODE GA:1,2 OP
	LOG Tutorial5b.err
	HIDE_HELP
)