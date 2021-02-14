add_executable(Tutorial1
	Tutorial1.cpp
)

target_link_libraries(Tutorial1 PUBLIC GraphicsSDK)

pvs_studio_add_target(TARGET Tutorial1.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE Tutorial1
	MODE GA:1,2 OP
	LOG Tutorial1.err
	HIDE_HELP
)