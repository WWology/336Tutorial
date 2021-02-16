add_subdirectory(models)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/MVP_VS.vert
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ColorFS.frag
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_library(Shader4c
	shader.h
	shader.cpp
)

add_executable(Tutorial4c Tutorial4c.cpp)

target_link_libraries(Tutorial4c PUBLIC
	GraphicsSDK
	Shader4c
)

pvs_studio_add_target(TARGET Tutorial4c.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial4c Shader4c
	MODE GA:1,2 OP
	LOG Tutorial4c.err
	HIDE_HELP
)