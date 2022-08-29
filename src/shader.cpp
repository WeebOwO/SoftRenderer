#include "shader.h"
#include "renderer.h"

//extern WindowCreaterInfo window_info;

Vertex vs_shader(Vertex a) {
    Light light({ 0.2f, 1.0f, 2.0f }, {1.0f, 1.0f, 1.0f});
    Mat4x4f model = global_context::shader_context->get_model_mat();
    Mat4x4f view = global_context::shader_context->get_view_mat();
    Mat4x4f normal_model = matrix_invert(model).Transpose();
    Mat4x4f viewport = matrix_set_viewport(600, 600);
    Mat4x4f projection = matrix_set_perspective(3.1415926f * 0.5f, 1.0f, 1.0f, 500.0f);
    Vertex temp;
    Vec4f pos = a.pos.xyz1() * model * projection;
    //将法线转化到世界坐标系下
    //计算漫反射光
    float diffuse = vector_dot(a.normal, vector_normalize(light.light_pos));
    diffuse = Max(0.0f, diffuse) + 0.1f;
    Vec3f color = light.light_color * a.color * diffuse;
    //透视除法将齐次坐标转换到欧式空间
    pos /= pos.w;
    //视口转换，将NDC空间的坐标映射到屏幕坐标
    pos = pos * viewport;
    temp.pos = pos.xyz();
    temp.color = color;
    return temp;
}

ShaderContext* ShaderContext::get_instance() {
    static ShaderContext* instance = new ShaderContext();
    return instance;
}
