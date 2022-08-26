#pragma once
#include <cmath>
#include <utility>
#include <initializer_list>
#include <algorithm>
#include <chrono>
#include <format>
#include <memory>

#include <SDL.h>

#include "math.h"
#include "shader.h"
#include "buffer.h"

struct WindowCreaterInfo {
    const char* title{ "CoreEngine" };
    int window_width{ 600 };
    int window_height{ 600 };
};

class Renderer {
private:
    SDL_Renderer* m_renderer{ nullptr };
    SDL_Window* m_window{ nullptr };
    Shader* m_shader{ nullptr };
    std::chrono::steady_clock::time_point m_last_tick_time_point{ std::chrono::steady_clock::now() };
private:
    void tickrender(float delta_time, const Buffer& buffer);
    void rasterize(const Vertex& a_trans, const Vertex& b_trans, const Vertex& c_trans);
    void draw_primitive(const Vertex& a, const Vertex& b, const Vertex& c);
    float cal_delta_time();

public:
    Renderer() = default;
    Renderer(const WindowCreaterInfo& window_info, Shader* shader);
    ~Renderer();
    void run(const Buffer& buffer);
    inline void set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const {  SDL_SetRenderDrawColor(m_renderer, r, g, b, a); }
      
};