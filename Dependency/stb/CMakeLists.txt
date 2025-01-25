cmake_minimum_required(VERSION 3.30)

project(stb)

set(CMAKE_C_STANDARD 11)

add_library(${PROJECT_NAME} src/stb_image.c)
target_include_directories(${PROJECT_NAME} PUBLIC include)
