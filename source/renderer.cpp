#include "renderer.h"

#include <SDL2/SDL.h>

#include <algorithm>
#include <execution>
#include <utility>
static bool IsTopLeft(const Vec2i& a, const Vec2i& b) {
  return ((a.y == b.y) && (a.x < b.x)) || (a.y > b.y);
}

ShaderContext Renderer::BarycentricInterplate(std::array<Vertex, 3>& vertices,
                                              const Vec3f& barycentric) {
  ShaderContext ret;
  ShaderContext& s0 = vertices[0].context_;
  ShaderContext& s1 = vertices[1].context_;
  ShaderContext& s2 = vertices[2].context_;

  std::array<float, 3> interplate_factor{barycentric.x, barycentric.y, barycentric.z};

  for (const auto& [key, value] : s0.varying_vec4f_) {
    Vec4f factor_0 = value * interplate_factor[0];
    Vec4f factor_1 = s1.varying_vec4f_[key] * interplate_factor[1];
    Vec4f factor_2 = s2.varying_vec4f_[key] * interplate_factor[2];
    ret.varying_vec4f_[key] = factor_0 + factor_1 + factor_2;
  }

  for (const auto& [key, value] : s0.varying_vec3f_) {
    Vec3f factor_0 = value * interplate_factor[0];
    Vec3f factor_1 = s1.varying_vec3f_[key] * interplate_factor[1];
    Vec3f factor_2 = s2.varying_vec3f_[key] * interplate_factor[2];
    ret.varying_vec3f_[key] = factor_0 + factor_1 + factor_2;
  }

  for (const auto& [key, value] : s0.varying_vec2f_) {
    Vec2f factor_0 = value * interplate_factor[0];
    Vec2f factor_1 = s1.varying_vec2f_[key] * interplate_factor[1];
    Vec2f factor_2 = s2.varying_vec2f_[key] * interplate_factor[2];
    ret.varying_vec2f_[key] = factor_0 + factor_1 + factor_2;
  }

  for (const auto& [key, value] : s0.varying_float_) {
    float factor_0 = value * interplate_factor[0];
    float factor_1 = s1.varying_float_[key] * interplate_factor[1];
    float factor_2 = s2.varying_float_[key] * interplate_factor[2];
    ret.varying_float_[key] = factor_0 + factor_1 + factor_2;
  }
  return ret;
}

void Renderer::DrawPrimitive(std::array<VertexAttrib, 3>& vs_input) {
  if (m_vertex_shader_ == nullptr || m_pixel_shader_ == nullptr) return;
  std::array<Vertex, 3> vertices;
  int width = m_window_width_, height = m_window_height_;
  int min_x, max_x, min_y, max_y;

  for (int i = 0; i < 3; ++i) {
    vertices[i].context_.Clear();
    // 运行Vertex Shader
    vertices[i].pos_ = m_vertex_shader_(vs_input[i], vertices[i].context_);

    // 齐次坐标裁减
    float w = vertices[i].pos_.w;
    if (w == 0.0) { return; }
    if (vertices[i].pos_.z < 0 || vertices[i].pos_.z > w) { return; };
    if (vertices[i].pos_.y < -w || vertices[i].pos_.y > w) { return; };
    if (vertices[i].pos_.x < -w || vertices[i].pos_.x > w) { return; };

    // 透视除法
    vertices[i].rhw_ = 1.0f / w;
    vertices[i].pos_ *= vertices[i].rhw_;

    // 映射到viewport
    vertices[i].spf_.x = (vertices[i].pos_.x + 1.0f) * width * 0.5f;
    vertices[i].spf_.y = (1.0f - vertices[i].pos_.y) * height * 0.5f;

    vertices[i].spi_.x = static_cast<int>(vertices[i].spf_.x + 0.5f);
    vertices[i].spi_.y = static_cast<int>(vertices[i].spf_.y + 0.5f);

    if (i == 0) {
      // 初始化
      min_x = max_x = Between(0, width - 1, vertices[i].spi_.x);
      min_y = max_y = Between(0, height - 1, vertices[i].spi_.y);
    } else {
      min_x = Between(0, width - 1, Min(min_x, vertices[i].spi_.x));
      max_x = Between(0, width - 1, Max(max_x, vertices[i].spi_.x));
      min_y = Between(0, height - 1, Min(min_y, vertices[i].spi_.y));
      max_y = Between(0, height - 1, Max(max_y, vertices[i].spi_.y));
    }
  }

  Vec4f v01 = vertices[1].pos_ - vertices[0].pos_;
  Vec4f v02 = vertices[2].pos_ - vertices[0].pos_;
  Vec4f normal = vector_cross(v01, v02);

  if (normal.z > 0.0f) { std::swap(vertices[2], vertices[1]); }

  if (normal.z == 0.0f) return;
  Vec2i p0 = vertices[0].spi_;
  Vec2i p1 = vertices[1].spi_;
  Vec2i p2 = vertices[2].spi_;

  float s = Abs(vector_cross(p1 - p0, p2 - p0));
  if (s < 0) return;

  bool TopLeft01 = IsTopLeft(p0, p1);
  bool TopLeft12 = IsTopLeft(p1, p2);
  bool TopLeft20 = IsTopLeft(p2, p0);

#pragma omp parallel for
  // 迭代三角形外接矩形的所有点
  std::vector<int> horizontal_vec(max_x - min_x + 1);

  [&horizontal_vec, min_x]() mutable {
    for(int i = 0; i < horizontal_vec.size(); ++i) {
      horizontal_vec[i] = min_x++;
    }
  }();

  for (int cy = min_y; cy <= max_y; cy++) {
    // 利于平行化STL foreach 进行加速
    std::for_each(std::execution::par, horizontal_vec.begin(), horizontal_vec.end(), [&](int cx) {
      Vec2f px = {(float)cx + 0.5f, (float)cy + 0.5f};
      // Edge Equation
      // 使用整数避免浮点误差，同时因为是左手系，所以符号取反
      int E01 = -(cx - p0.x) * (p1.y - p0.y) + (cy - p0.y) * (p1.x - p0.x);
      int E12 = -(cx - p1.x) * (p2.y - p1.y) + (cy - p1.y) * (p2.x - p1.x);
      int E20 = -(cx - p2.x) * (p0.y - p2.y) + (cy - p2.y) * (p0.x - p2.x);

      // 如果是左上边，用 E >= 0 判断合法，如果右下边就用 E > 0 判断合法
      // 这里通过引入一个误差 1 ，来将 < 0 和 <= 0 用一个式子表达
      if (E01 < (TopLeft01 ? 0 : 1)) return;  // 在第一条边后面
      if (E12 < (TopLeft12 ? 0 : 1)) return;  // 在第二条边后面
      if (E20 < (TopLeft20 ? 0 : 1)) return;  // 在第三条边后面

      // 三个端点到当前点的矢量
      Vec2f s0 = vertices[0].spf_ - px;
      Vec2f s1 = vertices[1].spf_ - px;
      Vec2f s2 = vertices[2].spf_ - px;

      // 重心坐标系：计算内部子三角形面积 a / b / c
      float a = Abs(vector_cross(s1, s2));  // 子三角形 Px-P1-P2 面积
      float b = Abs(vector_cross(s2, s0));  // 子三角形 Px-P2-P0 面积
      float c = Abs(vector_cross(s0, s1));  // 子三角形 Px-P0-P1 面积
      s = a + b + c;                        // 大三角形 P0-P1-P2 面积

      if (s == 0.0f) return;

      // 除以总面积，以保证：a + b + c = 1，方便用作插值系数
      a = a * (1.0f / s);
      b = b * (1.0f / s);
      c = c * (1.0f / s);

      // 计算当前点的 1/w，因 1/w 和屏幕空间呈线性关系，故直接重心插值
      // 因为透视投影最后一步会除以w
      float rhw = vertices[0].rhw_ * a + vertices[1].rhw_ * b + vertices[2].rhw_ * c;

      // 进行深度测试
      if (rhw < m_depth_buffer_[cy * m_window_width_ + cx]) return;
      m_depth_buffer_[cy * m_window_width_ + cx] = rhw;  // 记录 1/w 到深度缓存

      // 还原当前像素的 w
      float w = 1.0f / ((rhw != 0.0f) ? rhw : 1.0f);

      // 计算三个顶点插值 varying 的系数
      // 先除以各自顶点的 w 然后进行屏幕空间插值然后再乘以当前 w
      // 已经经过透视校正
      float c0 = vertices[0].rhw_ * a * w;
      float c1 = vertices[1].rhw_ * b * w;
      float c2 = vertices[2].rhw_ * c * w;

      ShaderContext input = BarycentricInterplate(vertices, Vec3f{c0, c1, c2});
      // 执行像素着色器
      Vec4f color = {0.0f, 0.0f, 0.0f, 0.0f};
      color = m_pixel_shader_(input);
      DrawPixel(cx, cy, color);
    });
  }
}

void Renderer::RenderPresent() {
  SDL_UpdateTexture(m_swap_texture_, nullptr, m_frame_buffer_, m_window_width_ * 4);
//  SDL_SetTextureBlendMode(m_swap_texture_, SDL_BLENDMODE_BLEND);
  SDL_RenderCopy(m_renderer_, m_swap_texture_, nullptr, nullptr);
  SDL_RenderPresent(m_renderer_);
}

Renderer::Renderer(const WindowInfo& window_info)
    : m_window_width_(window_info.width_), m_window_height_(window_info.height_) {
  m_window_ = SDL_CreateWindow(window_info.title_, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               window_info.width_, window_info.height_, SDL_WINDOW_RESIZABLE);
  m_renderer_ = SDL_CreateRenderer(m_window_, -1, 0);
  Resize(window_info.width_, window_info.height_);
}

Renderer::~Renderer() {
  SDL_DestroyWindow(m_window_);
  SDL_DestroyRenderer(m_renderer_);
  delete[] m_frame_buffer_;
}

void Renderer::DrawPixel(int x, int y, const Vec4f& color) {
  Vec4f my_color_vec = vector_clamp(color, 0.0f, 1.0f);
  auto r = static_cast<int>(my_color_vec.r * 255.0f);
  auto g = static_cast<int>(my_color_vec.g * 255.0f);
  auto b = static_cast<int>(my_color_vec.b * 255.0f);
  auto a = static_cast<int>(my_color_vec.a * 255.0f);

  uint32_t final_color = (a << 24) + (r << 16) + (g << 8) + b;
  *(m_frame_buffer_ + y * m_window_width_ + x) = final_color;
}

void Renderer::RenderClear() {
  SDL_SetRenderDrawColor(m_renderer_, 0, 0, 0, 255);
  SDL_RenderClear(m_renderer_);
  std::fill(m_depth_buffer_.begin(), m_depth_buffer_.end(), 0);
}

void Renderer::Resize(int width, int height) {
  ResizeDepthBuffer(width, height);
  ResizeFrameBuffer(width, height);
}

void Renderer::ResizeDepthBuffer(int width, int height) { m_depth_buffer_.resize(width * height); }

void Renderer::SetVertexShader(VertexShader vertex_shader) { m_vertex_shader_ = std::move(vertex_shader); }

void Renderer::SetPixelShader(PixelShader pixel_shader) { m_pixel_shader_ = std::move(pixel_shader); }

void Renderer::ResizeFrameBuffer(int width, int height) {
  m_swap_texture_ = SDL_CreateTexture(m_renderer_, SDL_PIXELFORMAT_ARGB8888,
                                      SDL_TEXTUREACCESS_STREAMING, width, height);
  m_frame_buffer_ = new uint32_t [width * height];
}
