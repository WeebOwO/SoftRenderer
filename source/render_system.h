#pragma once

#include <unordered_map>
#include "math.h"

class SDL_Renderer;
class SDL_Window;
class RuntimeContext;

struct WindowInfo {
  const char* title_{"HardCore"};
  int x_{0};
  int y_{0};
  int width_{900};
  int height_{600};
};

class Renderer {
 public:
  void RenderWithContext(const RuntimeContext& context);
  void RenderPresent();
  void RenderClear();
  void DrawPixel(int x, int y, Vec4f color);
  void ResizeDepthBuffer(int width, int height);
  Renderer() = delete;
  Renderer(const WindowInfo& window_info);
  ~Renderer();
 private:
  // 只是用来管理窗口和使用画点api
  SDL_Renderer* m_renderer_{nullptr};
  SDL_Window* m_window_{nullptr};
  std::vector<float> depth_buffer_;
};