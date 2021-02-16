add_subdirectory(models)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PerFragLightingVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PerFragLightingFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader5c
	shader.h
	shader.cpp
)

add_executable(Tutorial5c Tutorial5c.cpp)

target_link_libraries(Tutorial5c PUBLIC
	GraphicsSDK
	Shader5c
)

pvs_studio_add_target(TARGET Tutorial5c.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial5c Shader5c
	MODE GA:1,2 OP
	LOG Tutorial5c.err
	HIDE_HELP
)