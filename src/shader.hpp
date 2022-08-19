#pragma once

#include <SDL.h>

#include "renderer.hpp"
#include "math.hpp"

struct Transform {
    Mat4x4f model;
    Mat4x4f view;
    Mat4x4f projection;
};

Vertex vs_shader(Vertex a) {
    //Mat4x4f viewport = matrix_set_viewport(600, 600);
    //Vertex temp;
    //temp.pos = a.pos * viewport;
    ////std::cout << temp.pos;
    //temp.color = a.color;
    //return temp;
    return a;
}
