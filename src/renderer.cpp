#include <algorithm>
#include <cmath>
#include "renderer.h"

namespace utils {
    void BarycentricInterplate(ShaderVaryingData& output, const std::array<ShaderVaryingData, 3>& vertex_array, const Vec3f& barycentric) {
        Vec3f sreen_coord_a = vertex_array[0].scrren_coord, sreen_coord_b = vertex_array[1].scrren_coord, sreen_coord_c = vertex_array[2].scrren_coord;
        float z = 1 / (barycentric.x * (1 / sreen_coord_a.z) + barycentric.y * (1 / sreen_coord_b.z) + barycentric.z * (1 / sreen_coord_c.z));
        float a_trans_inverse_z = 1 / sreen_coord_a.z, b_trans_inverse_z = 1 / sreen_coord_b.z, c_trans_inverse_z = 1 / sreen_coord_c.z;
        // 颜色属性插值
        float r = (z * (barycentric.x * vertex_array[0].color.r * a_trans_inverse_z + barycentric.y * vertex_array[1].color.r * b_trans_inverse_z + barycentric.z * vertex_array[2].color.r * c_trans_inverse_z));
        float g = (z * (barycentric.x * vertex_array[0].color.g * a_trans_inverse_z + barycentric.y * vertex_array[1].color.g * b_trans_inverse_z + barycentric.z * vertex_array[2].color.g * c_trans_inverse_z));
        float b = (z * (barycentric.x * vertex_array[0].color.b * a_trans_inverse_z + barycentric.y * vertex_array[1].color.b * b_trans_inverse_z + barycentric.z * vertex_array[2].color.b * c_trans_inverse_z));
        output.color = Vec3f(r, g, b);
        // 世界坐标插值
        float world_x = (z * (barycentric.x * vertex_array[0].world_pos.x * a_trans_inverse_z + barycentric.y * vertex_array[1].world_pos.x * b_trans_inverse_z + barycentric.z * vertex_array[2].world_pos.x * c_trans_inverse_z));
        float world_y = (z * (barycentric.x * vertex_array[0].world_pos.y * a_trans_inverse_z + barycentric.y * vertex_array[1].world_pos.y * b_trans_inverse_z + barycentric.z * vertex_array[2].world_pos.y * c_trans_inverse_z));
        float world_z = (z * (barycentric.x * vertex_array[0].world_pos.z * a_trans_inverse_z + barycentric.y * vertex_array[1].world_pos.z * b_trans_inverse_z + barycentric.z * vertex_array[2].world_pos.z * c_trans_inverse_z));
        output.world_pos = Vec3f(world_x, world_y, world_z);
        // 法线坐标插值
        float normal_x = (z * (barycentric.x * vertex_array[0].normal.x * a_trans_inverse_z + barycentric.y * vertex_array[1].normal.x * b_trans_inverse_z + barycentric.z * vertex_array[2].normal.x * c_trans_inverse_z));
        float normal_y = (z * (barycentric.x * vertex_array[0].normal.y * a_trans_inverse_z + barycentric.y * vertex_array[1].normal.y * b_trans_inverse_z + barycentric.z * vertex_array[2].normal.y * c_trans_inverse_z));
        float normal_z = (z * (barycentric.x * vertex_array[0].normal.z * a_trans_inverse_z + barycentric.y * vertex_array[1].normal.z * b_trans_inverse_z + barycentric.z * vertex_array[2].normal.z * c_trans_inverse_z));
        output.normal = Vec3f(normal_x, normal_y, normal_z);
    }
}


void Renderer::TickRenderer(float delta_time, const Buffer& buffer) {
    static float rotate_speed = 0.5f;
    static float angle = 0.0f;
    static float move_speed = 10.0f;
    static float view_offset = 0.0f;
    angle += rotate_speed * delta_time;
    view_offset += move_speed * delta_time;
    RenderClear();
    auto&& vertex_buffer = buffer.verter_buffer;
    auto&& index_buffer = buffer.index_buffer;
    Mat4x4f offset = MatrixSetTranslate(0, 0, 2.5);
    Mat4x4f rotate = MatrixSetRotate(1, 1, 0, angle);
    Mat4x4f model = rotate * offset;
    Mat4x4f normal_mat = MatrixInvert(model).Transpose();
    Mat4x4f look_at = MatrixSetLookat({ 0, 0, -1 }, { 0, 0, 0 }, { 0, 1, 0 });
    Mat4x4f projection = MatrixSetPerspective(3.1415926f * 0.35f, 1.0f, 1.0f, 10.0f);
    Mat4x4f viewport = MatrixSetViewport(600, 600);
    global_context::shader_context->SetModelMatrix(model);
    global_context::shader_context->SetViewMatrix(look_at);
    global_context::shader_context->SetProjectionMatrix(projection);
    global_context::shader_context->SetNormalMatrix(normal_mat);
    global_context::shader_context->SetViewPortMatrix(viewport);
    for (auto&& index : index_buffer) {
        DrawPrimitive(vertex_buffer[index[0]], vertex_buffer[index[1]], vertex_buffer[index[2]]);
    }
    SDL_RenderPresent(m_renderer);
}

void Renderer::Run(const Buffer& buffer) {
    int quit = 0;
    SDL_Event event;
    while (!quit) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
                break;
            }
        }
        float delta_time = GetDeltaTime();
        TickRenderer(delta_time, buffer);
    }
}

void Renderer::SetColor(const Vec4f& color) {
    Vec4f my_color = VectorClamp(color, 0.0f, 1.0f);
    uint8_t r = static_cast<uint8_t>(my_color.r * 255.0f);
    uint8_t g = static_cast<uint8_t>(my_color.g * 255.0f);
    uint8_t b = static_cast<uint8_t>(my_color.b * 255.0f);
    uint8_t a = static_cast<uint8_t>(my_color.a * 255.0f);
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

void Renderer::RenderClear() {
    SetColor(Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
    SDL_RenderClear(m_renderer);
    for (auto&& depth_vec : m_depth_buffer) {
        std::fill(depth_vec.begin(), depth_vec.end(), 10000.0f);
    }
}

void Renderer::DrawPrimitive(const Vertex& a, const Vertex& b, const Vertex& c) {
    ShaderVaryingData a_trans = m_shader->RunVertexShader(a), b_trans = m_shader->RunVertexShader(b), c_trans = m_shader->RunVertexShader(c);
    // 齐次坐标裁剪,超出范围的话直接舍弃整个三角形
    std::array<ShaderVaryingData, 3> varying_data_array { a_trans, b_trans, c_trans };
    for(auto&& data : varying_data_array) {
        float x = data.homogeneous_coord.x;
        float y = data.homogeneous_coord.y;
        float z = data.homogeneous_coord.z;
        float w = data.homogeneous_coord.w;
        if (w == 0.0f) return;
        if (z < 0.0f || z > w) return;
        if (x < -w || x > w) return;
        if (y < -w || y > w) return;
    }

    // 透视除法转换为欧式坐标, 然后再通过视口变化转换到屏幕坐标系
    for (auto&& data : varying_data_array) {
        float reverse_w = 1.0f / data.homogeneous_coord.w;
        data.homogeneous_coord.x *= reverse_w;
        data.homogeneous_coord.y *= reverse_w;
        data.homogeneous_coord.z *= reverse_w;
        data.homogeneous_coord.w = 1.0f;
        data.scrren_coord = (data.homogeneous_coord * global_context::shader_context->GetViewPortMatrix()).xyz();
    }
    //求出三角形的AABB包围盒
    
    Vec3f sreen_coord_a = varying_data_array[0].scrren_coord, sreen_coord_b = varying_data_array[1].scrren_coord, sreen_coord_c = varying_data_array[2].scrren_coord;
    int min_x = static_cast<int>(std::min({ sreen_coord_a.x , sreen_coord_b.x, sreen_coord_c.x }));
    int min_y = static_cast<int>(std::min({ sreen_coord_a.y , sreen_coord_b.y, sreen_coord_c.y }));
    int max_x = static_cast<int>(std::max({ sreen_coord_a.x , sreen_coord_b.x, sreen_coord_c.x }));
    int max_y = static_cast<int>(std::max({ sreen_coord_a.y , sreen_coord_b.y, sreen_coord_c.y }));

    for (int x = min_x; x <= max_x; ++x) {
        for (int y = min_y; y <= max_y; ++y) {
            // 用重心坐标来判断当前点是不是在三角形内, https://zhuanlan.zhihu.com/p/65495373参考证明资料
            Vec3f x_part(sreen_coord_b.x - sreen_coord_a.x, sreen_coord_c.x - sreen_coord_a.x, sreen_coord_a.x - x);
            Vec3f y_part(sreen_coord_b.y - sreen_coord_a.y, sreen_coord_c.y - sreen_coord_a.y, sreen_coord_a.y - y);
            Vec3f u_part = VectorCross(x_part, y_part);
            Vec3f barycentric(1.0f - (u_part.x + u_part.y) / u_part.z, u_part.x / u_part.z, u_part.y / u_part.z);
            if (barycentric.x >= 0 && barycentric.y >= 0 && barycentric.z >= 0) {
                // 通过正确的深度来计算插值, 因为屏幕空间下直接利用重心坐标系的结果是不符合在观察坐标系的所看到的深度
                // 其根本原因是，对于深度来说，透视投影的变化对于深度(z)来说并非线性变化，所以投影过后的z值也不能简单的根据深度来进行插值
                float z = 1 / (barycentric.x * (1 / sreen_coord_a.z) + barycentric.y * (1 / sreen_coord_b.z) + barycentric.z * (1 / sreen_coord_c.z));
                // 进行深度测试, early-z, 未通过深度测试的片元则被丢弃
                if (y >= (int)m_depth_buffer.size() || x >= (int)m_depth_buffer[0].size() || m_depth_buffer[y][x] < z) continue;
                m_depth_buffer[y][x] = z;
                ShaderVaryingData fragment;
                utils::BarycentricInterplate(fragment, varying_data_array, barycentric);
                Vec4f color = m_shader->RunFragmentShader(fragment);
                SetColor(color);
                SDL_RenderDrawPoint(m_renderer, x, y);
            }
        }
    } 
}

float Renderer::GetDeltaTime() {
    float delta_time; 
    {
        using namespace std::chrono;
        steady_clock::time_point tick_time_point = steady_clock::now();
        duration<float> time_span = duration_cast<duration<float>>(tick_time_point - m_last_tick_time_point);
        delta_time = time_span.count();
        m_last_tick_time_point = tick_time_point;
    }
    return delta_time;
}

Renderer::Renderer(const WindowCreaterInfo& window_info, Shader* shader): m_shader(shader) {
    SDL_CreateWindowAndRenderer(window_info.window_width, window_info.window_height, SDL_WINDOW_RESIZABLE, &m_window, &m_renderer);
    SDL_SetWindowTitle(m_window, window_info.title);
    m_depth_buffer.resize(window_info.window_height, std::vector<float>(window_info.window_height, 10000.0f));
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}