cmake_minimum_required(VERSION 3.23)
project(SoftRenderer)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_executable(SoftRenderer
        src/buffer.h
        src/global.cpp
        src/global.h
        src/light.h
        src/main.cpp
        src/math.h
        src/renderer.cpp
        src/renderer.h
        src/shader.cpp
        src/shader.h)
