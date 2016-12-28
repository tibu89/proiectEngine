find_package(OpenGL REQUIRED)

# includes
include_directories(
	../external/glew-1.13.0/include/
	../external/glfw-3.2.1/include/GLFW/
	../external/glm/
	inc/
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
	assimp
)

add_definitions(
	-DGLEW_STATIC
)

set(CAMERA_FILES
	inc/Camera.h
	inc/FirstPersonCamera.h
	inc/ThirdPersonCamera.h
	src/FirstPersonCamera.cpp
	src/ThirdPersonCamera.cpp
)

set(PHYSICS_FILES
	inc/CollisionShapes.h
	inc/ContactGenerator.h
	inc/ContactResolver.h
	inc/PhysicsEngine.h
	inc/RigidBody.h
	src/ContactGenerator.cpp
	src/ContactResolver.cpp
	src/PhysicsEngine.cpp
	src/RigidBody.cpp
)

set(GRAPHICS_FILES
	inc/Mesh.h
	inc/Graphics.h
	inc/tempModels.h
	src/Graphics.cpp
)

set(INPUT_MANAGER_FILES
	inc/InputManager.h
	src/InputManager.cpp
)

source_group("Camera" FILES ${CAMERA_FILES})
source_group("Physics" FILES ${PHYSICS_FILES})
source_group("Graphics" FILES ${GRAPHICS_FILES})
source_group("InputManager" FILES ${INPUT_MANAGER_FILES})

add_library(AzureEngine
	${CAMERA_FILES}
	${PHYSICS_FILES}
	${GRAPHICS_FILES}
	${INPUT_MANAGER_FILES}
	shaders/basicPS.pixelshader
	shaders/basicVS.vertexshader
)

target_link_libraries(AzureEngine ${ALL_LIBS})