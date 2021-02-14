configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ModelSpaceVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ColorFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader3c
	shader.h
	shader.cpp
)

add_executable(Tutorial3c Tutorial3c.cpp)

target_link_libraries(Tutorial3c PUBLIC
	GraphicsSDK
	Shader3c
)

pvs_studio_add_target(TARGET Tutorial3c.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial3c Shader3c
	MODE GA:1,2 OP
	LOG Tutorial3c.err
	HIDE_HELP
)