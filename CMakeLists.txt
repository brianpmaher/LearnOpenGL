cmake_minimum_required(VERSION 3.30)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)

set(EXAMPLE_PATH Source/Example)

add_subdirectory(Dependency/glfw-3.4)

add_subdirectory(Dependency/glad)

#
# OpenGL example applications
#

# 1. Hello World
add_executable(${PROJECT_NAME}_HelloWorld ${EXAMPLE_PATH}/1_HelloWorld/Main.cpp)

# 2. Hello Window
add_executable(${PROJECT_NAME}_HelloWindow ${EXAMPLE_PATH}/2_HelloWindow/Main.cpp)
target_compile_definitions(${PROJECT_NAME}_HelloWindow PRIVATE GLFW_INCLUDE_NONE)
target_link_libraries(${PROJECT_NAME}_HelloWindow PRIVATE glfw glad)

# 3. Clear
add_executable(${PROJECT_NAME}_Clear ${EXAMPLE_PATH}/3_Clear/Main.cpp)
target_compile_definitions(${PROJECT_NAME}_Clear PRIVATE GLFW_INCLUDE_NONE)
target_link_libraries(${PROJECT_NAME}_Clear PRIVATE glfw glad)

# 4. Hello Triangle
add_executable(${PROJECT_NAME}_HelloTriangle ${EXAMPLE_PATH}/4_HelloTriangle/Main.cpp)
target_compile_definitions(${PROJECT_NAME}_HelloTriangle PRIVATE GLFW_INCLUDE_NONE)
target_link_libraries(${PROJECT_NAME}_HelloTriangle PRIVATE glfw glad)
