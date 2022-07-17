#pragma once
#include <SDL.h>
#include <math.h>
#include <utility>
#include <initializer_list>
#include <algorithm>

#include "mesh.hpp"
#include "math.hpp"


class Renderer {
private:
    int m_window_width, m_window_height;
    SDL_Renderer* m_renderer;
    SDL_Window* m_window;

private:
    void tickrender(const StaticMesh& mesh) {
        for (auto&& index_seq : mesh.indexbuffer) {
            int index_a = index_seq[0], index_b = index_seq[1], index_c = index_seq[2];
            draw_primitive(mesh.vertexbuffer[index_a], mesh.vertexbuffer[index_b], mesh.vertexbuffer[index_c]);
        }
    }

    void draw_primitive(const std::vector<float>& point_a, const std::vector<float>& point_b, const std::vector<float>& point_c) {

        Vec2f a(point_a[0], point_a[1]); Vec2f b(point_b[0], point_b[1]); Vec2f c(point_c[0], point_c[1]); 
        Vec3f a_color(point_a[2], point_a[3], point_a[4]), b_color(point_b[2], point_b[3], point_b[4]), c_color(point_c[2], point_c[3], point_c[4]);

        //每个顶点调用一次Vertex shader
        
        int min_x = std::min({ a.x, b.x, c.x });
        int min_y = std::min({ a.y, b.y, c.y });
        int max_x = std::max({ a.x, b.x, c.x });
        int max_y = std::max({ a.y, b.y, c.y });

        //光栅化部分
        SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
        for (int x = min_x; x <= max_x; ++x) {
            for (int y = min_y; y <= max_y; ++y) {
                //用重心坐标来判断当前点是不是在三角形内, https://zhuanlan.zhihu.com/p/65495373参考证明资料
                Vec3f x_part(b.x - a.x, c.x - a.x, a.x - x);
                Vec3f y_part(b.y - a.y, c.y - a.y, a.y - y);
                Vec3f u_part = vector_cross(x_part, y_part);
                Vec3f barycentric(1.0f - (u_part.x + u_part.y) / u_part.z, u_part.x / u_part.z, u_part.y / u_part.z);
                if (barycentric.x > 0 && barycentric.y > 0 && barycentric.z > 0) {
                    Uint8 r = static_cast<Uint8>(barycentric.x * a_color.r + barycentric.y * b_color.r + barycentric.z * c_color.r);
                    Uint8 g = static_cast<Uint8>(barycentric.x * a_color.g + barycentric.y * b_color.g + barycentric.z * c_color.g);
                    Uint8 b = static_cast<Uint8>(barycentric.x * a_color.b + barycentric.y * b_color.b + barycentric.z * c_color.b);
                    SDL_SetRenderDrawColor(m_renderer, r, g, b, 255);
                    //------每个pixel调用一次fragment_shader------------
                    SDL_RenderDrawPoint(m_renderer, x, y);
                }
            }
        }
    }

public:
    Renderer(int window_width, int window_height):m_window_width(window_width), m_window_height(window_height) {
        SDL_CreateWindowAndRenderer(window_width, window_height, SDL_WINDOW_RESIZABLE, &m_window, &m_renderer);
    }

    void run(const StaticMesh& mesh) {
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
            tickrender(mesh);
            SDL_RenderPresent(m_renderer);
        }
    }

    void set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
    }

    ~Renderer() {
        SDL_DestroyWindow(m_window);
        SDL_DestroyRenderer(m_renderer);
    }
};