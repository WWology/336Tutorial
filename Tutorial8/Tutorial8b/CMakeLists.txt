configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PerFragLightingVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PerFragLightingFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader8b
	shader.h
	shader.cpp
)

add_executable(Tutorial8b Tutorial8b.cpp)

target_link_libraries(Tutorial8b PUBLIC
	GraphicsSDK
	Shader8b
)

pvs_studio_add_target(TARGET Tutorial8b.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial8b Shader8b
	MODE GA:1,2 OP
	LOG Tutorial8b.err
	HIDE_HELP
)