cmake_minimum_required(VERSION 3.30)

project(glad)

set(CMAKE_C_STANDARD 11)

add_library(${PROJECT_NAME} src/glad.c)

target_include_directories(${PROJECT_NAME} PUBLIC include)
