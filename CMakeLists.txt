cmake_minimum_required(VERSION 3.10)

project("Open GL education")
set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(GLFW3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

set(LIBS glfw glm ${OPENGL_LIBRARIES} ${GLEW_LIBRARY})
add_executable(gl src/main.cpp)

#include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
#target_link_libraries(gl ${opengl_libraries} ${GLEW_LIBRARY}) 

target_link_libraries(gl ${LIBS})
