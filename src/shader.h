#pragma once
#include <functional>
#include <memory>

#include <SDL.h>
#include "math.h"
#include "light.h"



struct Vertex {
    Vec3f pos;
    Vec3f normal;
    Vec3f color;
}; 

struct ShaderVaryingData {
    Vec4f homogeneous_coord;
    Vec3f world_pos;
    Vec3f normal;
    Vec3f color;
    Vec3f scrren_coord;
};

struct Fragment{
    Vec3f world_pos;
    Vec3f normal;
    Vec2f scrren_pos_float;
    Vec3f color;
};


class ShaderContext {
private:
    Mat4x4f m_model_mat, m_view_mat, m_projection_mat, m_normal_mat, m_viewport_mat;
    Light m_light;
    ShaderContext() = default;
public:
    ShaderContext(const ShaderContext& other) = delete;
    ShaderContext& operator=(const ShaderContext& other) = delete;

    static ShaderContext* GetInstance();

    inline Mat4x4f GetModelMatrix();
    inline Mat4x4f GetViewMatrix();
    inline Mat4x4f GetProjectionMatrix();
    inline Mat4x4f GetTransformMatrix();
    inline Mat4x4f GetNormalMatrix();
    inline Mat4x4f GetViewPortMatrix();
    inline Light GetLight();

    inline void SetModelMatrix(const Mat4x4f& model);
    inline void SetViewMatrix(const Mat4x4f& view);
    inline void SetProjectionMatrix(const Mat4x4f& projection);
    inline void SetNormalMatrix(const Mat4x4f& normal_mat);
    inline void SetLight(const Light& light);
    inline void SetViewPortMatrix(const Mat4x4f& viewport);
};

typedef std::function<ShaderVaryingData(const Vertex&)> VertexShader;
typedef std::function<Vec4f(const ShaderVaryingData&)> FragmentShader;

class Shader {
private:
    VertexShader m_vertex_shader;
    FragmentShader m_fragment_shader;
public:
    Shader() = default;
    ~Shader() = default;
    Shader(VertexShader vs_shader, FragmentShader fs_shader);
    ShaderVaryingData RunVertexShader(const Vertex& vertex);
    Vec4f RunFragmentShader(const ShaderVaryingData& varying_data);
    inline void SetVertexShader(VertexShader vs_shader);
    inline void SetFragmentShader(FragmentShader fs_shader);
    inline FragmentShader GetFragmentShader();
    inline VertexShader GetVertexShader();
};

inline Mat4x4f ShaderContext::GetModelMatrix() { return m_model_mat; }
inline Mat4x4f ShaderContext::GetViewMatrix() { return m_view_mat; }
inline Mat4x4f ShaderContext::GetProjectionMatrix() { return m_projection_mat; }
inline Mat4x4f ShaderContext::GetNormalMatrix() { return m_normal_mat; }
inline Mat4x4f ShaderContext::GetTransformMatrix() { return m_model_mat * m_view_mat * m_projection_mat; }
inline Mat4x4f ShaderContext::GetViewPortMatrix() { return m_viewport_mat; }
inline Light ShaderContext::GetLight() { return m_light; }
inline void ShaderContext::SetModelMatrix(const Mat4x4f& model) { m_model_mat = model; }
inline void ShaderContext::SetViewMatrix(const Mat4x4f& view) { m_view_mat = view; }
inline void ShaderContext::SetProjectionMatrix(const Mat4x4f& projection) { m_projection_mat = projection; }
inline void ShaderContext::SetNormalMatrix(const Mat4x4f& normal) { m_normal_mat = normal; }
inline void ShaderContext::SetLight(const Light& light) { m_light = light; }
inline void ShaderContext::SetViewPortMatrix(const Mat4x4f& viewport_mat) { m_viewport_mat = viewport_mat; }

inline Shader::Shader(VertexShader vs_shader, FragmentShader fs_shader) : m_vertex_shader(vs_shader), m_fragment_shader(fs_shader) {}
inline void Shader::SetVertexShader(VertexShader vs_shader) { m_vertex_shader = vs_shader; }
inline void Shader::SetFragmentShader(FragmentShader fs_shader) { m_fragment_shader = fs_shader; }
inline FragmentShader Shader::GetFragmentShader() { return m_fragment_shader; }
inline VertexShader Shader::GetVertexShader() { return m_vertex_shader; }
