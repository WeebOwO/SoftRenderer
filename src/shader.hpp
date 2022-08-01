#pragma once

#include <functional>
#include <vector>

#include "math.hpp"
using Vertex_shader = std::function<void(std::vector<Vertex>&)>;
using Fragment_shader = std::function<void(std::vector<Fragment>&)>;

struct Vertex {
    Vec3f pos;
    Vec4i color;
};


struct Fragment {
    Vec2f screen_pos;
    Vec3f world_pos;
};

struct Transform {
    Mat4x4f model_mat;
    Mat4x4f view_mat;
    Mat4x4f project_mat;
    Mat4x4f view_port_mat;
};

class Shader {
private:
    Vertex_shader m_vertex_shader;
    Fragment_shader m_fragment_shader;
    
public:
    Shader() = delete;
    Shader(Vertex_shader vertex_shader, Fragment_shader fragment_shader) :m_vertex_shader(vertex_shader), m_fragment_shader(fragment_shader) {}

    void set_vertexshader(Vertex_shader vertex_shader) {
        m_vertex_shader = vertex_shader;
    }

    void set_fragmentshaer(Fragment_shader fragment_shader) {
        m_fragment_shader = fragment_shader;
    }
    
    void run(std::vector<std::vector<Vertex>>& vertex_buffer, std::vector<std::vector<int>>& index_buffer) {
        
    }
};

