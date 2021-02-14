configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ColorVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ColorFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader2d
	shader.h
	shader.cpp
)

add_executable(Tutorial2d Tutorial2d.cpp)

target_link_libraries(Tutorial2d PUBLIC
	GraphicsSDK
	Shader2d
)

pvs_studio_add_target(TARGET Tutorial2d.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial2d Shader2d
	MODE GA:1,2 OP
	LOG Tutorial2d.err
	HIDE_HELP
)