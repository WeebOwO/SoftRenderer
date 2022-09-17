#include "shader.h"
#include "global.h"

Light light({ 0.0f, 0.0f, 0.0f }, {1.0f, 1.0f, 1.0f});

ShaderVaryingData MyVertexShader(Vertex a) {
    Mat4x4f model = global_context::shader_context->GetModelMatrix();
    Mat4x4f normal_model = global_context::shader_context->GetNormalMatrix();
    Mat4x4f transform = global_context::shader_context->GetTransformMatrix();
    ShaderVaryingData temp;
    temp.homogeneous_coord = a.pos.xyz1() * transform;
    temp.world_pos = (a.pos.xyz1() * model).xyz();
    temp.normal = (a.normal.xyz1() * normal_model).xyz();
    temp.color = a.color;
    return temp;
}

Vec4f MyFragmentShader(const ShaderVaryingData& input) {
    Vec3f world_pos = input.world_pos, normal = input.normal;
    Vec3f light_pos = light.light_pos, light_color = light.light_color;
    Vec3f lightdir = VectorNormalize(light_pos - world_pos);
    float diffuse_intensity = 0.8f * Max(VectorDot(normal, lightdir), 0.0f);
    float ambient_instensity = 0.2f;
    Vec3f color = (diffuse_intensity + ambient_instensity) * light_color * input.color;
    return color.xyz1();
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
