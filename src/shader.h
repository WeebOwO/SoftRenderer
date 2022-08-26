#pragma once
#include <functional>

#include <SDL.h>

#include "math.h"

struct Vertex {
    Vec4f pos;
    Vec4i color;
};

struct Fragment{
    Vec3f world_pos;
    Vec3f view_pos;
    Vec2f scrren_pos_float;
    Vec2i scrren_pos_int;
    Vec4i color;
};

struct Transform {
    Mat4x4f model;
    Mat4x4f view;
    Mat4x4f projection;
};

using VertexShader = std::function<Vertex(Vertex)>;
using FragmentShader = std::function<void(Fragment)>;

class Shader {
private:
    VertexShader m_vs_shader;
public:
    Shader(VertexShader vs_shader) : m_vs_shader(vs_shader) {};
    inline void set_vertex_shader(VertexShader vs_shader) { m_vs_shader = vs_shader; }
    inline VertexShader get_vs_shader() { return m_vs_shader; }
};

Vertex vs_shader(Vertex a);

