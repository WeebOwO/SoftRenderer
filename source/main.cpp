#include <vector>

#include "engine.h"
#include "math.h"
#include "runtime_context.h"
#include "shader.h"

// Test data
static std::vector<Vec3f> position = {
    {0.0f, 0.7f, 0.0f},
    {-0.6f, -0.2f, 0.0f},
    {0.6f, -0.2f, 0.0f}
};
static std::vector<Vec4f> color = {
    {1.0f, 0.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f, 1.0f}
};
static std::vector<std::array<int, 3>> index_buffer = {{0, 1, 2}};

Vec4f SimpleVertexShader(int index, ShaderContext& output) {
  const auto& runtime_context = RuntimeContext::GetInstance();
  output.varying_vec4f[kColor] = runtime_context.model_info_.color_[index];
  return runtime_context.model_info_.pos_[index].xyz1();
}

Vec4f SimplePixelShader(ShaderContext& input) { return input.varying_vec4f[kColor]; }

int main() {
  auto&& engine = Application::GetInstance("Core", 900, 600);
  auto&& runtime_context = RuntimeContext::GetInstance();
  // 初始化状态
  runtime_context.model_info_ = ModelInfo({position, index_buffer, color});
  runtime_context.vertex_shader_ = SimpleVertexShader;
  runtime_context.pixel_shader_ = SimplePixelShader;
  // 开始运行
  engine->Run();
  return 0;
}