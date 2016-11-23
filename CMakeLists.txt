# CMake entry point
cmake_minimum_required (VERSION 2.6)
project (MG3DTR)

find_package(OpenGL REQUIRED)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()

# external dependencies
add_subdirectory (external)

# includes
include_directories(
	external/glew-1.13.0/include/
	external/glfw-3.2.1/include/GLFW/
	external/glm/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_definitions(
	-DGLEW_STATIC
)



