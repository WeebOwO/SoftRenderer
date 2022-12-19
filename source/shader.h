#pragma once

#include <functional>
#include <unordered_map>

#include "math.h"

struct ShaderContext {
  std::unordered_map<int, float> varying_float;  // 浮点数 varying 列表
  std::unordered_map<int, Vec2f> varying_vec2f;  // 二维矢量 varying 列表
  std::unordered_map<int, Vec3f> varying_vec3f;  // 三维矢量 varying 列表
  std::unordered_map<int, Vec4f> varying_vec4f;  // 四维矢量 varying 列表
  void Clear();
};

struct Vertex {
  ShaderContext context_;
  float rhw_;
  Vec4f pos_;
  Vec2f spf_;
  Vec2i spi_;
};

using VertexShader = std::function<Vec4f(int index, ShaderContext& output)>;
using PixelShader = std::function<Vec4f(ShaderContext& input)>;
