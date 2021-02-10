add_executable(Tutorial2d Tutorial2d.cpp)
target_link_libraries(Tutorial2d PUBLIC GraphicsSDK)

pvs_studio_add_target(TARGET Tutorial2d.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial2d
	MODE GA:1,2 OP
	LOG Tutorial2d.err
)