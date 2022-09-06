#include "shader.h"
#include "global.h"

Light light({ 0.0f, 0.0f, 0.0f }, {1.0f, 1.0f, 1.0f});

ShaderVaryingData vs_shader(Vertex a) {
    Mat4x4f model = global_context::shader_context->GetModelMatrix();
    Mat4x4f view = global_context::shader_context->GetViewMatrix();
    Mat4x4f projection = global_context::shader_context->GetProjectionMatrix();
    Mat4x4f normal_model = global_context::shader_context->GetNormalMatrix();
    Mat4x4f transform = global_context::shader_context->GetTransformMatrix();
    ShaderVaryingData temp;
    temp.homogeneous_coord = a.pos.xyz1() * transform;
    temp.world_pos = (a.pos.xyz1() * model).xyz();
    temp.normal = (a.normal.xyz1() * normal_model).xyz();
    temp.color = a.color;
    return temp;
}

ShaderContext* ShaderContext::GetInstance() {
    static ShaderContext* instance = new ShaderContext();
    return instance;
}

ShaderVaryingData Shader::RunVertexShader(const Vertex& a) {
    return m_vertex_shader(a);
}

Vec4f Shader::RunFragmentShader(const ShaderVaryingData& varying_data) {
    return m_fragment_shader(varying_data);
}
