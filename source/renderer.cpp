#include "renderer.h"

#include <SDL2/SDL.h>

#include <array>
#include <iostream>

#include "runtime_context.h"
#include "shader.h"

void Renderer::RenderWithContext(const RuntimeContext& context) {
  RenderClear();
  const auto& faces = context.model_info_.faces_;
  const auto& pos = context.model_info_.pos_;
  static std::array<Vertex, 3> vertices;
  for (const auto& face : faces) {
    int min_x, max_x, min_y, max_y;
    for (int i = 0; i < 3; ++i) {
      vertices[i].context_.Clear();
      // 运行Vertex Shader
      vertices[i].pos_ = context.vertex_shader_(face[i], vertices[i].context_);
      // 齐次坐标裁减

      // 映射到viewport
      int width = 0, height = 0;
      SDL_GetWindowSize(m_window_, &width, &height);
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
    // 对cache友好一点
    for (int y = min_y; y <= max_y; ++y) {
      for (int x = min_x; x <= max_x; ++x) {
        // 利用重心坐标法来判断点是不是在三角形内
        // https://zhuanlan.zhihu.com/p/65495373 参考证明资料
        float px = static_cast<float>(x + 0.5f);
        float py = static_cast<float>(y + 0.5f);
        Vec3f x_part(vertices[1].spf_.x - vertices[0].spf_.x,
                     vertices[2].spf_.x - vertices[0].spf_.x, vertices[0].spf_.x - px);
        Vec3f y_part(vertices[1].spf_.y - vertices[0].spf_.y,
                     vertices[2].spf_.y - vertices[0].spf_.y, vertices[0].spf_.y - py);

        Vec3f u_part = vector_cross(x_part, y_part);
        Vec3f barycentric(1.0f - (u_part.x + u_part.y) / u_part.z, u_part.x / u_part.z,
                          u_part.y / u_part.z);

        if (barycentric.x >= 0 && barycentric.y >= 0 && barycentric.z >= 0) {
          std::array<float, 3> interplate_factor{barycentric.x, barycentric.y, barycentric.z};
          ShaderContext input;
          ShaderContext& s0 = vertices[0].context_;
          ShaderContext& s1 = vertices[1].context_;
          ShaderContext& s2 = vertices[2].context_;
          for (const auto& [key, value] : s0.varying_vec4f) {
            Vec4f factor_0 = value * interplate_factor[0];
            Vec4f factor_1 = s1.varying_vec4f[key] * interplate_factor[1];
            Vec4f factor_2 = s2.varying_vec4f[key] * interplate_factor[2];
            input.varying_vec4f[key] = factor_0 + factor_1 + factor_2;
          }

          Vec4f color = context.pixel_shader_(input);
          DrawPixel(x, y, color);
        }
      }
    }
  }
  RenderPresent();
}

void Renderer::RenderPresent() { SDL_RenderPresent(m_renderer_); }

Renderer::Renderer(const WindowInfo& window_info) {
  SDL_CreateWindowAndRenderer(window_info.width_, window_info.height_, SDL_WINDOW_RESIZABLE,
                              &m_window_, &m_renderer_);
}

Renderer::~Renderer() {
  SDL_DestroyWindow(m_window_);
  SDL_DestroyRenderer(m_renderer_);
}

void Renderer::DrawPixel(int x, int y, Vec4f color) {
  Vec4f my_color = vector_clamp(color, 0.0f, 1.0f);
  auto r = static_cast<uint8_t>(my_color.r * 255.0f);
  auto g = static_cast<uint8_t>(my_color.g * 255.0f);
  auto b = static_cast<uint8_t>(my_color.b * 255.0f);
  auto a = static_cast<uint8_t>(my_color.a * 255.0f);
  SDL_SetRenderDrawColor(m_renderer_, r, g, b, a);
  SDL_RenderDrawPoint(m_renderer_, x, y);
}

void Renderer::RenderClear() {
  SDL_SetRenderDrawColor(m_renderer_, 0, 0, 0, 255);
  SDL_RenderClear(m_renderer_);
}
