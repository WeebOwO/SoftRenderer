#pragma once

#include <SDL.h>

#include "renderer.hpp"
#include "math.hpp"

Vertex vs_shader(Vertex a, Transform t) {
    Mat4x4f viewport = matrix_set_viewport(600, 600);
    Mat4x4f projection = matrix_set_perspective(3.14159 / 2.0f, 1.0f, 1.0f, 10.0f);
    Vertex temp;
    temp.pos = a.pos * projection;
    temp.pos = temp.pos / temp.pos.w;
    temp.pos = temp.pos * viewport;
    temp.color = a.color;
    return temp;
}
