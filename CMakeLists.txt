cmake_minimum_required(VERSION 3.30)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_PATH Source)
set(EXAMPLE_PATH ${SOURCE_PATH}/Example)
set(COMMON_PATH ${SOURCE_PATH}/Common)
set(COMMON_LIB ${PROJECT_NAME}_Common)

#
# Dependencies
#

add_subdirectory(Dependency/glfw-3.4)

add_subdirectory(Dependency/glad)

#
# Common library
#

add_library(${COMMON_LIB} ${COMMON_PATH}/Shader.cpp ${COMMON_PATH}/Window.cpp)
target_include_directories(${COMMON_LIB} INTERFACE ${SOURCE_PATH})
target_compile_definitions(${COMMON_LIB} PUBLIC GLFW_INCLUDE_NONE)
target_link_libraries(${COMMON_LIB} PUBLIC glfw glad)

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

# 5. Hello Quad
add_executable(${PROJECT_NAME}_HelloQuad ${EXAMPLE_PATH}/5_HelloQuad/Main.cpp)
target_compile_definitions(${PROJECT_NAME}_HelloQuad PRIVATE GLFW_INCLUDE_NONE)
target_link_libraries(${PROJECT_NAME}_HelloQuad PRIVATE glfw glad)

# 6. Shaders
add_executable(${PROJECT_NAME}_Shaders ${EXAMPLE_PATH}/6_Shaders/Main.cpp)
target_link_libraries(${PROJECT_NAME}_Shaders PRIVATE ${COMMON_LIB})
