#pragma once
#include <SDL.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <memory>
#include <vector>

#include "buffer.h"
#include "global.h"
#include "math.h"
#include "shader.h"

struct WindowCreaterInfo {
  const char *title{"CoreEngine"};
  int window_width{600};
  int window_height{600};
};

class Renderer {
 private:
  SDL_Renderer *m_renderer{nullptr};
  SDL_Window *m_window{nullptr};
  Shader *m_shader{nullptr};
  std::vector<std::vector<float>> m_depth_buffer;
  std::chrono::steady_clock::time_point m_last_tick_time_point{
      std::chrono::steady_clock::now()};

 private:
  void TickRenderer(float delta_time, const Buffer &buffer);
  void DrawPrimitive(const Vertex &a, const Vertex &b, const Vertex &c);
  float GetDeltaTime();

 public:
  Renderer() = default;
  Renderer(const WindowCreaterInfo &window_info, Shader *shader);
  ~Renderer();
  void Run(const Buffer &buffer);
  void SetColor(const Vec4f &color);
  void RenderClear();
};