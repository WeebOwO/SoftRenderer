#pragma once

#include <functional>
#include <unordered_map>

#include "other/math.h"

class ShaderContext {
 public:
  std::unordered_map<int, float> varying_float_;  // 浮点数 varying 列表
  std::unordered_map<int, Vec2f> varying_vec2f_;  // 二维矢量 varying 列表
  std::unordered_map<int, Vec3f> varying_vec3f_;  // 三维矢量 varying 列表
  std::unordered_map<int, Vec4f> varying_vec4f_;  // 四维矢量 varying 列表
  void Clear();
};

struct VertexAttrib {
  Vec3f pos;
  Vec3f normal;
  Vec2f uv;
};

struct Vertex {
  ShaderContext context;
  float rhw;
  Vec4f pos;
  Vec2f spf;
  Vec2i spi;
};

using VertexShader = std::function<Vec4f(VertexAttrib& vs_input, ShaderContext& output)>;
using PixelShader = std::function<Vec4f(ShaderContext& input)>;
