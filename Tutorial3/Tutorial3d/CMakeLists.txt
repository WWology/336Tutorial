configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ColorVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ColorFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader3d
	shader.h
	shader.cpp
)

add_executable(Tutorial3d Tutorial3d.cpp)

target_link_libraries(Tutorial3d PUBLIC
	GraphicsSDK
	Shader3d
)

pvs_studio_add_target(TARGET Tutorial3d.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial3d Shader3d
	MODE GA:1,2 OP
	LOG Tutorial3d.err
	HIDE_HELP
)