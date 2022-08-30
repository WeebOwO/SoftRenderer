#include <algorithm>
#include <cmath>

#include "renderer.h"

void Renderer::tickrender(float delta_time, const Buffer& buffer) {
    static float rotate_speed = 0.5f;
    static float angle = 0.0f;
    static float move_speed = 10.0f;
    static float view_offset = 0.0f;
    angle += rotate_speed * delta_time;
    view_offset += move_speed * delta_time;
    render_clear();
    auto&& vertex_buffer = buffer.verter_buffer;
    auto&& index_buffer = buffer.index_buffer;
    Mat4x4f offset = matrix_set_translate(0, 0, 2.5);
    Mat4x4f rotate = matrix_set_rotate(1, 1, 0, angle);
    Mat4x4f model = rotate * offset;
    Mat4x4f look_at = matrix_set_lookat({ 0, 0, -1 }, { 0, 0, 0 }, { 0, 1, 0 });
    Mat4x4f projection = matrix_set_perspective(3.1415926f * 0.35f, 1.0f, 1.0f, 10.0f);
    for (auto&& index : index_buffer) {
        global_context::shader_context->set_model_mat(model);
        global_context::shader_context->set_view_mat(look_at);
        global_context::shader_context->set_projection_mat(projection);
        draw_primitive(vertex_buffer[index[0]], vertex_buffer[index[1]], vertex_buffer[index[2]]);
    }
    SDL_RenderPresent(m_renderer);
}

void Renderer::run(const Buffer& buffer) {
    int quit = 0;
    SDL_Event event;
    while (!quit) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
                break;
            }
        }
        float delta_time = cal_delta_time();
        tickrender(delta_time, buffer);
    }
}

void Renderer::render_clear() {
    set_color(0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
    for (auto&& depth_vec : m_depth_buffer) {
        std::fill(depth_vec.begin(), depth_vec.end(), 10000.0f);
    }
}

void Renderer::rasterize(const Vertex& a_trans, const Vertex& b_trans, const Vertex& c_trans) {
    //求出三角形的AABB包围盒
    int min_x = static_cast<int>(std::min({ a_trans.pos.x, b_trans.pos.x, c_trans.pos.x }));
    int min_y = static_cast<int>(std::min({ a_trans.pos.y, b_trans.pos.y, c_trans.pos.y }));
    int max_x = static_cast<int>(std::max({ a_trans.pos.x, b_trans.pos.x, c_trans.pos.x }));
    int max_y = static_cast<int>(std::max({ a_trans.pos.y, b_trans.pos.y, c_trans.pos.y }));
    
    for (int x = min_x; x <= max_x; ++x) {
        for (int y = min_y; y <= max_y; ++y) {
            //用重心坐标来判断当前点是不是在三角形内, https://zhuanlan.zhihu.com/p/65495373参考证明资料
            Vec3f x_part(b_trans.pos.x - a_trans.pos.x, c_trans.pos.x - a_trans.pos.x, a_trans.pos.x - x);
            Vec3f y_part(b_trans.pos.y - a_trans.pos.y, c_trans.pos.y - a_trans.pos.y, a_trans.pos.y - y);
            Vec3f u_part = vector_cross(x_part, y_part);
            Vec3f barycentric(1.0f - (u_part.x + u_part.y) / u_part.z, u_part.x / u_part.z, u_part.y / u_part.z);
            if (barycentric.x >= 0 && barycentric.y >= 0 && barycentric.z >= 0) {
                //通过正确的深度来计算插值, 因为屏幕空间下直接利用重心坐标系的结果是不符合在观察坐标系的所看到的深度
                //其根本原因是，对于深度来说，透视投影的变化对于深度(z)来说并非线性变化，所以投影过后的z值也不能简单的根据深度来进行插值
                float z = 1 / (barycentric.x * (1 / a_trans.pos.z) + barycentric.y * (1 / b_trans.pos.z) + barycentric.z * (1 / c_trans.pos.z));
                //进行深度测试, early-z
                if (y >= m_depth_buffer.size() || x >= m_depth_buffer[0].size() || m_depth_buffer[y][x] < z) continue;
                m_depth_buffer[y][x] = z;
                float a_trans_inverse_z = 1 / a_trans.pos.z, b_trans_inverse_z = 1 / b_trans.pos.z, c_trans_inverse_z = 1 / c_trans.pos.z;
                Uint8 r = static_cast<Uint8>(z * (barycentric.x * a_trans.color.r * a_trans_inverse_z + barycentric.y * b_trans.color.r * b_trans_inverse_z + barycentric.z * c_trans.color.r * c_trans_inverse_z) * 255);
                Uint8 g = static_cast<Uint8>(z * (barycentric.x * a_trans.color.g * a_trans_inverse_z + barycentric.y * b_trans.color.g * b_trans_inverse_z + barycentric.z * c_trans.color.g * c_trans_inverse_z) * 255);
                Uint8 b = static_cast<Uint8>(z * (barycentric.x * a_trans.color.b * a_trans_inverse_z + barycentric.y * b_trans.color.b * b_trans_inverse_z + barycentric.z * c_trans.color.b * c_trans_inverse_z) * 255);
                SDL_SetRenderDrawColor(m_renderer, r, g, b, 255);
                SDL_RenderDrawPoint(m_renderer, x, y);
            }
        }
    }
}

void Renderer::draw_primitive(const Vertex& a, const Vertex& b, const Vertex& c) {
    VertexShader vertex_shader = m_shader->get_vs_shader();
    set_color(255, 0, 0, 255);
    Vertex a_trans = vertex_shader(a), b_trans = vertex_shader(b), c_trans = vertex_shader(c);
    
    /*SDL_RenderDrawLine(m_renderer, a_trans.pos.x, a_trans.pos.y, b_trans.pos.x, b_trans.pos.y);
    SDL_RenderDrawLine(m_renderer, b_trans.pos.x, b_trans.pos.y, c_trans.pos.x, c_trans.pos.y);
    SDL_RenderDrawLine(m_renderer, c_trans.pos.x, c_trans.pos.y, a_trans.pos.x, a_trans.pos.y);*/
    rasterize(a_trans, b_trans, c_trans);
}

float Renderer::cal_delta_time() {
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