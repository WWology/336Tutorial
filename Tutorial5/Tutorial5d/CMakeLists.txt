add_subdirectory(models)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/MultiLightsVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/MultiLightsFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader5d
	shader.h
	shader.cpp
)

add_executable(Tutorial5d Tutorial5d.cpp)

target_link_libraries(Tutorial5d PUBLIC
	GraphicsSDK
	Shader5d
)

pvs_studio_add_target(TARGET Tutorial5d.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial5d Shader5d
	MODE GA:1,2 OP
	LOG Tutorial5d.err
	HIDE_HELP
)