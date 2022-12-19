#include <SDL2/SDL.h>
#include "renderer.h"
#include "model.h"

const int VARYING_UV = 0;
const int VARYING_EYE = 1;

int main() {
  Model model("../asset/diablo3_pose.obj");
  WindowInfo window_info = {"Core", 0, 0, 900, 600};
  auto renderer = Renderer(window_info);

  Vec3f eye_pos = {0, -0.5, 1.7};
  Vec3f eye_at = {0, 0, 0};
  Vec3f eye_up = {0, 1, 0};
  Vec3f light_dir = {1, 1, 0.85};
  float perspective = 3.1415926f * 0.5f;

  Mat4x4f mat_model = matrix_set_scale(1, 1, 1);
  Mat4x4f mat_view = matrix_set_lookat(eye_pos, eye_at, eye_up);
  Mat4x4f mat_proj = matrix_set_perspective(perspective, 6 / 8.0, 1.0, 500.0f);
  Mat4x4f mat_mvp = mat_model * mat_view * mat_proj;
  Mat4x4f mat_model_it = matrix_invert(mat_model).Transpose();

  struct { Vec3f pos; Vec3f normal; Vec2f uv; } vs_input[3];

  renderer.SetVertexShader([&](int index, ShaderContext& output) {
    Vec4f pos = vs_input[index].pos.xyz1() * mat_mvp;
    // 将顶点位置从模型空间转换为世界坐标系
    Vec3f pos_world = (vs_input[index].pos.xyz1() * mat_model).xyz();
    // 计算模型顶点到眼睛的方向
    Vec3f eye_dir = eye_pos - pos_world;
    output.varying_vec2f[VARYING_UV] = vs_input[index].uv;
    output.varying_vec3f[VARYING_EYE] = eye_dir;
    return pos;
  });

  renderer.SetPixelShader([&](ShaderContext& input) {
    Vec2f uv = input.varying_vec2f[VARYING_UV];
    // 模型上当前点到眼睛的方向
    Vec3f eye_dir = input.varying_vec3f[VARYING_EYE];
    // 归一化光照方向
    Vec3f l = vector_normalize(light_dir);
    // 法向贴图取出法向并转换为世界坐标系
    Vec3f n = (model.normal(uv).xyz1() * mat_model_it).xyz();
    // 从模型中取出当前点的高光参数
    float s = model.Specular(uv);
    // 计算反射光线
    Vec3f r = vector_normalize(n * vector_dot(n, l) * 2.0f - l);
    // 计算高光
    float p = Saturate(vector_dot(r, eye_dir));
    float spec = Saturate(pow(p, s * 20) * 0.05);
    // 综合光照强度
    float intense = Saturate(vector_dot(n, l)) + 0.2f + spec;
    Vec4f color = model.diffuse(uv);
    return color * intense;
  });

  for(bool running=true; running;) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          running = false;
          break;
      }
    }

    renderer.RenderClear();
    for (int i = 0; i < model.nfaces(); i++) {
      // 设置三个顶点的输入，供 VS 读取
      for (int j = 0; j < 3; j++) {
        vs_input[j].pos = model.vert(i, j);
        vs_input[j].uv = model.uv(i, j);
        vs_input[j].normal = model.normal(i, j);
      }
      // 绘制三角形
      renderer.DrawPrimitive();
    }
    renderer.RenderPresent();
  }

  return 0;
}
