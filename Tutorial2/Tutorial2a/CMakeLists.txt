add_executable(Tutorial2a Tutorial2a.cpp)
target_link_libraries(Tutorial2a PUBLIC GraphicsSDK)

pvs_studio_add_target(TARGET Tutorial2a.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial2a
	MODE GA:1,2 OP
	LOG Tutorial2a.err
)