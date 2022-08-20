#pragma once
#include <cmath>
#include <utility>
#include <initializer_list>
#include <algorithm>
#include <functional>

#include <SDL.h>

#include "math.hpp"

struct Vertex {
    Vec4f pos;
    Vec4i color;
};

struct Transform {
    Mat4x4f model;
    Mat4x4f view;
    Mat4x4f projection;
};

class Renderer {
private:
    int m_window_width, m_window_height;
    SDL_Renderer* m_renderer;
    SDL_Window* m_window;
    std::function<Vertex(Vertex, Transform)> vertex_shader;
    int lastframe = 0, current_frame = 0;
private:
    void tickrender(const std::vector<Vertex>& vertex_buffer, const std::vector<std::vector<int>>& index_buffer) {
        for (auto&& index_seq : index_buffer) {
            int index_a = index_seq[0], index_b = index_seq[1], index_c = index_seq[2];
            draw_primitive(vertex_buffer[index_a], vertex_buffer[index_b], vertex_buffer[index_c]);
        }
    }

    void draw_primitive(const Vertex& a, const Vertex& b, const Vertex& c) {
        //Vertex shader调用
        Mat4x4f model = matrix_set_identity();
        Mat4x4f view = matrix_set_identity();
        Mat4x4f projection = matrix_set_identity();
        Transform t(model, view, projection);
        Vertex a_trans = vertex_shader(a, t), b_trans = vertex_shader(b, t), c_trans = vertex_shader(c, t);

        int min_x = std::min({ a_trans.pos.x, b_trans.pos.x, c_trans.pos.x });
        int min_y = std::min({ a_trans.pos.y, b_trans.pos.y, c_trans.pos.y });
        int max_x = std::max({ a_trans.pos.x, b_trans.pos.x, c_trans.pos.x });
        int max_y = std::max({ a_trans.pos.y, b_trans.pos.y, c_trans.pos.y });

        //光栅化部分
        SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(m_renderer, a.pos.x, a.pos.y, b.pos.x, b.pos.y);
        SDL_RenderDrawLine(m_renderer, b.pos.x, b.pos.y, c.pos.x, c.pos.y);
        SDL_RenderDrawLine(m_renderer, c.pos.x, c.pos.y, a.pos.x, a.pos.y);
        for (int x = min_x; x <= max_x; ++x) {
            for (int y = min_y; y <= max_y; ++y) {
                //用重心坐标来判断当前点是不是在三角形内, https://zhuanlan.zhihu.com/p/65495373参考证明资料
                Vec3f x_part(b_trans.pos.x - a_trans.pos.x, c_trans.pos.x - a_trans.pos.x, a_trans.pos.x - x);
                Vec3f y_part(b_trans.pos.y - a_trans.pos.y, c_trans.pos.y - a_trans.pos.y, a_trans.pos.y - y);
                Vec3f u_part = vector_cross(x_part, y_part);
                Vec3f barycentric(1.0f - (u_part.x + u_part.y) / u_part.z, u_part.x / u_part.z, u_part.y / u_part.z);
                if (barycentric.x > 0 && barycentric.y > 0 && barycentric.z > 0) {
                    float z = 1 / (barycentric.x * (1 / a_trans.pos.z) + barycentric.y * (1 / b_trans.pos.z) + barycentric.z * (1 / c_trans.pos.z));
                    //通过正确的深度来计算插值, 因为屏幕空间下直接利用重心坐标系的结果是不符合在观察坐标系的所看到的深度
                    //其根本原因是，对于深度来说，透视投影的变化对于深度(z)来说并非线性变化，所以投影过后的z值也不能简单的根据深度来进行插值
                    float a_trans_inverse_z = 1 / a_trans.pos.z, b_trans_inverse_z = 1 / b_trans.pos.z, c_trans_inverse_z = 1 / c_trans.pos.z;
                    Uint8 r = static_cast<Uint8>(z * (barycentric.x * a_trans.color.r * a_trans_inverse_z + barycentric.y * b_trans.color.r * b_trans_inverse_z + barycentric.z * c_trans.color.r * c_trans_inverse_z));
                    Uint8 g = static_cast<Uint8>(z * (barycentric.x * a_trans.color.g * a_trans_inverse_z + barycentric.y * b_trans.color.g * b_trans_inverse_z + barycentric.z * c_trans.color.g * c_trans_inverse_z));
                    Uint8 b = static_cast<Uint8>(z * (barycentric.x * a_trans.color.b * a_trans_inverse_z + barycentric.y * b_trans.color.b * b_trans_inverse_z + barycentric.z * c_trans.color.b * c_trans_inverse_z));
                    SDL_SetRenderDrawColor(m_renderer, r, g, b, 255);
                    SDL_RenderDrawPoint(m_renderer, x, y);
                }
            }
        }
    }

public:
    Renderer() = delete;
    Renderer(int window_width, int window_height):m_window_width(window_width), m_window_height(window_height) {
        SDL_CreateWindowAndRenderer(window_width, window_height, SDL_WINDOW_RESIZABLE, &m_window, &m_renderer);
    }

    void run(const std::vector<Vertex>& vertex_buffer, const std::vector<std::vector<int>>& index_buffer) {
        SDL_Event event;
        bool quit = false;
        while (!quit) {
            if (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }
            }
            //画之前将帧缓存设置为背景颜色。
            SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
            SDL_RenderClear(m_renderer);
            tickrender(vertex_buffer, index_buffer);
            SDL_RenderPresent(m_renderer);
        }
    }

    void set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
    }

    void set_vertexshader(std::function<Vertex(Vertex, Transform)> vs_shader) {
        vertex_shader = vs_shader;
    }
    //析构函数
    ~Renderer() {
        SDL_DestroyWindow(m_window);
        SDL_DestroyRenderer(m_renderer);
    }
};