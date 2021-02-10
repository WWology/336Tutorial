cmake_minimum_required(VERSION 3.0.0)
project(336Tutorial VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic -lpthread)
SET(CMAKE_EXE_LINKER_FLAGS   "-L/usr/local/lib")
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/PVS-Studio.cmake)


find_package(OpenGL REQUIRED)

find_library(glew
	libGLEW.a
	NAMES glew2 GLEW GLEW2
	DOC "GLEW Library"
	REQUIRED
)

find_package(glfw3 3.3 REQUIRED)
# find_library(glfw3
# 	libglfw3.a
# 	NAMES glfw3 GLFW GLFW3
# 	DOC "GLFW Library"
# 	REQUIRED
# )

find_library(glm
	libglm_static.a
	NAMES glm GLM
	PATHS "GraphicsSDK/glm/glm/"
	DOC "GLM Library"
	REQUIRED
)

find_library(AntTweakBar
	libAntTweakBar.a
	NAMES AntTweakBar anttweakbar ANTTWEAKBAR
	PATHS "GraphicsSDK/AntTweakBar/lib/"
	DOC "AntTweakBar Library"
	REQUIRED
)

find_library(assimp
	libassimp.so
	NAMES assimp ASSIMP
	DOC "Assimp Library"
	REQUIRED
)

include_directories(
	# GraphicsSDK/glew-2.1.0/include/GL/
	GraphicsSDK/glfw-3.3.2/include/GLFW/
	GraphicsSDK/glm/glm/
	GraphicsSDK/assimp-5.0.1/include/assimp/
	GraphicsSDK/AntTweakBar/include/
)

add_library(GraphicsSDK INTERFACE)

target_link_libraries(GraphicsSDK INTERFACE
	OpenGL::GL
	# ${glfw3}
	glfw
	${glew}
	${glm}
	${AntTweakBar}
	${assimp}
)

add_subdirectory(Tutorial1)
add_subdirectory(Tutorial2)
add_subdirectory(Tutorial3)
add_subdirectory(Tutorial4)
add_subdirectory(Tutorial5)
add_subdirectory(Tutorial6)
add_subdirectory(Tutorial7)
add_subdirectory(Tutorial8)