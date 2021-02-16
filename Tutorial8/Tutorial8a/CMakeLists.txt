add_subdirectory(models)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PerFragLightingVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PerFragLightingFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader8a
	shader.h
	shader.cpp
)

add_executable(Tutorial8a Tutorial8a.cpp)

target_link_libraries(Tutorial8a PUBLIC
	GraphicsSDK
	Shader8a
)

pvs_studio_add_target(TARGET Tutorial8a.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial8a Shader8a
	MODE GA:1,2 OP
	LOG Tutorial8a.err
	HIDE_HELP
)