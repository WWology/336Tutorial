add_subdirectory(models)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PerFragLightingVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ToonShadingFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader7c
	shader.h
	shader.cpp
)

add_executable(Tutorial7c Tutorial7c.cpp)

target_link_libraries(Tutorial7c PUBLIC
	GraphicsSDK
	Shader7c
)

pvs_studio_add_target(TARGET Tutorial7c.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial7c Shader7c
	MODE GA:1,2 OP
	LOG Tutorial7c.err
	HIDE_HELP
)