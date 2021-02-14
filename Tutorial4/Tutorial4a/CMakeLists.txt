configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ModelSpaceVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ColorFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader4a
	shader.h
	shader.cpp
)

add_executable(Tutorial4a Tutorial4a.cpp)

target_link_libraries(Tutorial4a PUBLIC
	GraphicsSDK
	Shader4a
)

pvs_studio_add_target(TARGET Tutorial4a.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial4a Shader4a
	MODE GA:1,2 OP
	LOG Tutorial4a.err
	HIDE_HELP
)