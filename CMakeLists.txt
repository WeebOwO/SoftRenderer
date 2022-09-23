cmake_minimum_required(VERSION 3.23)
project(SoftRenderer)
#用于指定SDL2的路径，最后运行的时候记得把SDL2.dll放到可执行文件的路径下
include_directories(dependency/SDL2-2.24.0/include/)
link_directories(dependency/SDL2-2.24.0/lib/x86)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

file(GLOB SRC_FILES
"${PROJECT_SOURCE_DIR}/src/*.h"
"${PROJECT_SOURCE_DIR}/src/*.cpp")

file(GLOB LIB_FILES "${PROJECT_SOURCE_DIR}/dependency/SDL2-2.24.0/lib/x86/*.lib")

add_executable(SoftRenderer ${SRC_FILES})
target_link_libraries(SoftRenderer ${LIB_FILES})