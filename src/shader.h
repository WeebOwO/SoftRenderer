#pragma once
#include <functional>
#include <memory>

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

using VertexShader = std::function<Vertex(Vertex)>;
using FragmentShader = std::function<void(Fragment)>;

class ShaderContext {
private:
    Mat4x4f m_model, m_view, m_projection;
    ShaderContext() = default;
    ShaderContext(const Mat4x4f& model, const Mat4x4f& view, const Mat4x4f& projection) : m_model(model), m_view(view), m_projection(projection) {}
public:
    ShaderContext(const ShaderContext& other) = delete;
    ShaderContext& operator=(const ShaderContext& other) = delete;

    static ShaderContext* get_instance();

    inline Mat4x4f get_model_mat() { return m_model; }
    inline Mat4x4f get_view_mat() { return m_view; }
    inline Mat4x4f get_projection_mat() { return m_projection; }
    inline Mat4x4f get_transform() { return m_model * m_view * m_projection; }

    inline void set_model_mat(const Mat4x4f& model) { m_model = model; }
    inline void set_view_mat(const Mat4x4f& view) { m_view = view; }
    inline void set_projection_mat(const Mat4x4f& projection) { m_projection = projection; }

};

class Shader {
private:
    VertexShader m_vs_shader;

public:
    ~Shader() = default;
    Shader(VertexShader vs_shader) : m_vs_shader(vs_shader) {}

    inline void set_vertex_shader(VertexShader vs_shader) { m_vs_shader = vs_shader; }
    inline VertexShader get_vs_shader() { return m_vs_shader; }
};

Vertex vs_shader(Vertex a);

