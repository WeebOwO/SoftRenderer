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
#include "window.h"
#include "shader.h"
#include "buffer.h"

class Renderer {
private:
    SDL_Renderer* m_renderer;
    std::unique_ptr<Window> m_window;
    std::unique_ptr<Shader> m_shader;
    std::chrono::steady_clock::time_point m_last_tick_time_point{ std::chrono::steady_clock::now() };
private:
    void tickrender(float delta_time, const Buffer& buffer);
    void rasterize(const Vertex& a_trans, const Vertex& b_trans, const Vertex& c_trans);
    void draw_primitive(const Vertex& a, const Vertex& b, const Vertex& c);
    float cal_delta_time();

public:
    Renderer() = default;
    ~Renderer();
    Renderer(Window* window, Shader* shader);

    void run(const Buffer& buffer);

    inline void set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const {  SDL_SetRenderDrawColor(m_renderer, r, g, b, a); }
      
};