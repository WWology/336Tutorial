add_executable(Tutorial2c Tutorial2c.cpp)
target_link_libraries(Tutorial2c PUBLIC GraphicsSDK)

pvs_studio_add_target(TARGET Tutorial2c.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial2c
	MODE GA:1,2 OP
	LOG Tutorial2c.err
)