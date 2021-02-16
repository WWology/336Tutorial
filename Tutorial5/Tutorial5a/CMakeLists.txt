configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PerVertexLightingVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ColorFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader5a
	shader.h
	shader.cpp
)

add_executable(Tutorial5a Tutorial5a.cpp)

target_link_libraries(Tutorial5a PUBLIC
	GraphicsSDK
	Shader5a
)

pvs_studio_add_target(TARGET Tutorial5a.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial5a Shader5a
	MODE GA:1,2 OP
	LOG Tutorial5a.err
	HIDE_HELP
)