#include "shader.h"
#include "renderer.h"

//extern WindowCreaterInfo window_info;

Vertex vs_shader(Vertex a) {
    Mat4x4f model = global_context::shader_context->get_model_mat();
    Mat4x4f viewport = matrix_set_viewport(600, 600);
    Mat4x4f projection = matrix_set_perspective(3.14159f / 2.0f, 1.0f, 1.0f, 10.0f);
    Vertex temp;
    
    temp.pos = a.pos * model * projection;
    temp.pos = temp.pos / temp.pos.w;
    temp.pos = temp.pos * viewport;
    temp.color = a.color;
    return temp;
}

ShaderContext* ShaderContext::get_instance() {
    static ShaderContext* instance = new ShaderContext();
    return instance;
}
