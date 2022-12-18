cmake_minimum_required(VERSION 3.23)
project(SoftRenderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

file(GLOB SRC_FILES
"${PROJECT_SOURCE_DIR}/source/*.h"
"${PROJECT_SOURCE_DIR}/source/*.cpp")

link_libraries(SDL2)
add_executable(SoftRenderer ${SRC_FILES})
