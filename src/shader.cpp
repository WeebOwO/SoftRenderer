
#include "shader.h"
#include "renderer.h"
#include "math.h"
//extern WindowCreaterInfo window_info;

Light light({ 0.0f, 0.0f, 0.0f }, {1.0f, 1.0f, 1.0f});
Vertex vs_shader(Vertex a) {
    Mat4x4f model = global_context::shader_context->get_model_mat();
    Mat4x4f view = global_context::shader_context->get_view_mat();
    Mat4x4f projection = global_context::shader_context->get_projection_mat();
    Mat4x4f viewport = matrix_set_viewport(600, 600);

    //将法线转化到世界坐标系下
    Mat4x4f normal_model = matrix_invert(model).Transpose();

    Vertex temp;
    Vec4f pos = a.pos.xyz1() * model * view * projection;
    a.normal = (a.normal.xyz1() * normal_model).xyz();
    //计算漫反射光和环境光
    float diffuse_intensity = Max(vector_dot(a.normal, vector_normalize(light.light_pos - pos.xyz())), 0.f);
    Vec3f diffuse_color = diffuse_intensity * light.light_color;
    Vec3f amibinet_color = 0.2f * light.light_color;
    Vec3f color = diffuse_color + amibinet_color;
    color.r = Min(1.0f, color.r);
    color.g = Min(1.0f, color.g);
    color.b = Min(1.0f, color.b);
    //透视除法将齐次坐标转换到欧式空间
    pos /= pos.w;
    //视口转换，将NDC空间的坐标映射到屏幕坐标
    pos = pos * viewport;
    temp.pos = pos.xyz();
    temp.color = a.color * color;
    return temp;
}

Fragment fs_shader(Fragment b) {

    return Fragment();
}

ShaderContext* ShaderContext::get_instance() {
    static ShaderContext* instance = new ShaderContext();
    return instance;
}
