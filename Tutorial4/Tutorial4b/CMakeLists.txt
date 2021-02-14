configure_file(${CMAKE_CURRENT_SOURCE_DIR}/MVP_VS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ColorFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader4b
	shader.h
	shader.cpp
)

add_executable(Tutorial4b Tutorial4b.cpp)

target_link_libraries(Tutorial4b PUBLIC
	GraphicsSDK
	Shader4b
)

pvs_studio_add_target(TARGET Tutorial4b.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial4b Shader4b
	MODE GA:1,2 OP
	LOG Tutorial4b.err
	HIDE_HELP
)