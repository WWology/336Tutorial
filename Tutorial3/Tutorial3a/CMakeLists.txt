configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ModelSpaceVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ColorFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader3a
	shader.h
	shader.cpp
)

add_executable(Tutorial3a Tutorial3a.cpp)

target_link_libraries(Tutorial3a PUBLIC
	GraphicsSDK
	Shader3a
)

pvs_studio_add_target(TARGET Tutorial3a.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial3a Shader3a
	MODE GA:1,2 OP
	LOG Tutorial3a.err
	HIDE_HELP
)