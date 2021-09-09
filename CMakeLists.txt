cmake_minimum_required(VERSION 3.20)
project(untitled4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
add_subdirectory(glad/)
add_subdirectory(include/glm/)
add_subdirectory(include/assimp/)



add_executable(untitled4 main.cpp src/shader/Shader.cpp src/shader/Shader.h src/data/buffer/VBO.cpp src/data/buffer/VBO.h src/data/buffer/EBO.cpp src/data/buffer/EBO.h src/data/VAO.cpp src/data/VAO.h include/stb/stb_image.h include/stb/stb.cpp src/data/texture/Texture.cpp src/data/texture/Texture.h src/spatial/camera/Camera.cpp src/spatial/camera/Camera.h src/object/Mesh.cpp src/object/Mesh.h src/object/Model.cpp src/object/Model.h src/object/material/Material.cpp src/object/material/Material.h src/spatial/scene/Scene.cpp src/spatial/scene/Scene.h src/spatial/scene/node/Node.cpp src/spatial/scene/node/Node.h)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL)

target_link_libraries(${PROJECT_NAME} PRIVATE glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)