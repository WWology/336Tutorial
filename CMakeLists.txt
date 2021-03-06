cmake_minimum_required(VERSION 3.0.0)
project(336Tutorial VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0072 NEW)
cmake_policy(SET CMP0079 NEW)

add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic -lpthread)
SET(CMAKE_EXE_LINKER_FLAGS   "-L/usr/local/lib")
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/PVS-Studio.cmake)


find_package(OpenGL REQUIRED)

set(GLEW_USE_STATIC_LIBS ON)
find_package(GLEW REQUIRED)

find_package(glfw3 3.3 REQUIRED)

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

find_package(assimp REQUIRED)

include_directories(
	${OPENGL_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${assimp_INCLUDE_DIR}
	GraphicsSDK/glm/
	GraphicsSDK/AntTweakBar/include/
)

add_library(GraphicsSDK INTERFACE)

target_link_libraries(GraphicsSDK INTERFACE
	OpenGL::GL
	GLEW::glew_s
	glfw
	${glm}
	${AntTweakBar}
	assimp
)

add_subdirectory(Tutorial1)
add_subdirectory(Tutorial2)
add_subdirectory(Tutorial3)
add_subdirectory(Tutorial4)
add_subdirectory(Tutorial5)
add_subdirectory(Tutorial6)
add_subdirectory(Tutorial7)
add_subdirectory(Tutorial8)