cmake_minimum_required(VERSION 3.30)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)

set(EXAMPLE_PATH Source/Example)

add_executable(${PROJECT_NAME}_HelloWorld ${EXAMPLE_PATH}/HelloWorld/Main.cpp)
