configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SpColorVS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ColorFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader
	shader.h
	shader.cpp
)

add_executable(Tutorial2c Tutorial2c.cpp)

target_link_libraries(Tutorial2c PUBLIC
	GraphicsSDK
	Shader
)

pvs_studio_add_target(TARGET Tutorial2c.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial2c
	MODE GA:1,2 OP
	LOG Tutorial2c.err
)