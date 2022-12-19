#pragma once

#include <array>
#include <unordered_map>

#include "bitmap.h"
#include "math.h"
#include "shader.h"

class SDL_Renderer;
class SDL_Window;
class SDL_Texture;
class scene;

struct WindowInfo {
  const char* title_{"HardCore"};
  int x_{0};
  int y_{0};
  int width_{900};
  int height_{600};
};

class Renderer {
 public:
  void RenderScene(const scene& scene);
  void RenderPresent();
  void DrawPrimitive();
  void RenderClear();
  void DrawPixel(int x, int y, const Vec4f& color);
  void Resize(int width, int height);
  void ResizeDepthBuffer(int width, int height);
  void ResizeFrameBuffer(int width, int height);
  void SetVertexShader(VertexShader vertex_shader);
  void SetPixelShader(PixelShader pixel_shader);
  Renderer() = delete;
  explicit Renderer(const WindowInfo& window_info);
  ~Renderer();
 private:
  ShaderContext BarycentricInterplate(std::array<Vertex, 3>& vertices, const Vec3f& barycentric);
 private:
  // 只是用来管理窗口的运行环境
  const int m_window_width_{900}, m_window_height_{600};
  SDL_Renderer* m_renderer_{nullptr};
  SDL_Window* m_window_{nullptr};
  SDL_Texture* m_swap_texture_{nullptr};
  uint32_t m_frame_buffer_[900 * 600];
  std::vector<float> m_depth_buffer_;
  VertexShader m_vertex_shader_;
  PixelShader m_pixel_shader_;
};