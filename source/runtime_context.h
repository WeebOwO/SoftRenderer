#pragma once

#include <memory>
#include <vector>
#include <array>
#include "math.h"
#include "shader.h"

class ShaderContext;

struct ModelInfo {
  std::vector<Vec3f> pos_;
  std::vector<std::array<int, 3>> faces_;
  std::vector<Vec4f> color_;
};

class RuntimeContext {
 public:
  static RuntimeContext& GetInstance();
  ModelInfo model_info_;
  VertexShader vertex_shader_ {nullptr};
  PixelShader pixel_shader_ {nullptr};
};
