add_subdirectory(models)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PerFragLightingVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/GoochShadingFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader7d
	shader.h
	shader.cpp
)

add_executable(Tutorial7d Tutorial7d.cpp)

target_link_libraries(Tutorial7d PUBLIC
	GraphicsSDK
	Shader7d
)

pvs_studio_add_target(TARGET Tutorial7d.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial7d Shader7d
	MODE GA:1,2 OP
	LOG Tutorial7d.err
	HIDE_HELP
)