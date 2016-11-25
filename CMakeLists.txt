# CMake entry point
cmake_minimum_required (VERSION 2.6)
project (AzureEngine)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()

message ("cxx Flags: " ${CMAKE_CXX_FLAGS})

# external dependencies
add_subdirectory (engine)
add_subdirectory (external)

include_directories(
	external/glew-1.13.0/include/
	external/glfw-3.2.1/include/GLFW/
	external/glm/
	engine/inc/
)

add_executable(testApp main.cpp)

target_link_libraries(testApp AzureEngine)