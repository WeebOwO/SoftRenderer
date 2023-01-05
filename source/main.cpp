#include <SDL2/SDL.h>

#include "other/model.h"
#include "renderer.h"

constexpr int VARYING_UV = 0;
constexpr int VARYING_EYE = 1;
constexpr int VARYING_DEPTH = 2;

int main() {
  Model model("../obj/diablo3_pose.obj");
  WindowInfo window_info = {"Core", 0, 0, 900, 600};
  auto renderer = Renderer(window_info);

  Vec3f eye_pos = {0, 0, 2};
  Vec3f eye_at = {0, 0, 0};
  Vec3f eye_up = {0, 1, 0};
  Vec3f light_dir = {1, 1, 0.85};
  float perspective = 3.1415926f * 0.5f;

  // 变换矩阵
  Mat4x4f mat_model = matrix_set_rotate(0, 1, 0, 0);
  Mat4x4f mat_view = matrix_set_lookat(eye_pos, eye_at, eye_up);
  Mat4x4f mat_proj = matrix_set_perspective(perspective, 9.0 / 6.0, 1.0, 500.0f);
  Mat4x4f mat_mvp = mat_model * mat_view * mat_proj;
  Mat4x4f mat_model_it = matrix_invert(mat_model).Transpose();

  renderer.SetVertexShader([&](VertexAttrib vs_input, ShaderContext& output) -> Vec4f {
    Vec4f pos = vs_input.pos.xyz1() * mat_mvp;
    Vec3f pos_world = (vs_input.pos.xyz1() * mat_model).xyz();
    Vec3f eye_dir = eye_pos - pos_world;
    output.varying_vec2f_[VARYING_UV] = vs_input.uv;
    output.varying_float_[VARYING_DEPTH] = vs_input.pos.z;
    output.varying_vec3f_[VARYING_EYE] = eye_dir;
    return pos;
  });

  renderer.SetPixelShader([&](ShaderContext& input) {
    Vec2f uv = input.varying_vec2f_[VARYING_UV];
    Vec3f eye_dir = input.varying_vec3f_[VARYING_EYE];
    Vec4f base_color = model.diffuse(uv);
    Vec3f l = vector_normalize(light_dir);
    Vec3f normal = (model.normal(uv).xyz1() * mat_model_it).xyz();
    Vec3f r = vector_normalize(normal * vector_dot(normal, l) * 2.0f - l);
    float s = model.Specular(uv);
    float p = Saturate(vector_dot(r, eye_dir));
    float spec_intensity = Saturate(pow(p, s * 10) * 0.05);

    float diffuse_intensity = vector_dot(l, normal);
    float ambient_intensity = 0.1;
    // test color
    //    Vec4f ambient_color = ambient_intensity * base_color;
    //    Vec4f diffuse_color = diffuse_intensity * base_color;
    //    Vec4f specular_color = spec_intensity * base_color;
    Vec4f output_color = (diffuse_intensity + ambient_intensity + spec_intensity) * base_color;
    return vector_clamp(output_color);
  });

  std::array<VertexAttrib, 3> vs_inputs;

  for (bool running = true; running;) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          running = false;
          break;
      }
    }
    renderer.RenderClear();

    for (int i = 0; i < model.nfaces(); ++i) {
      for (int j = 0; j < 3; ++j) {
        vs_inputs[j].pos = model.vert(i, j);
        vs_inputs[j].uv = model.uv(i, j);
        vs_inputs[j].normal = model.normal(i, j);
      }
      renderer.DrawPrimitive(vs_inputs);
    }
    renderer.RenderPresent();
    SDL_Delay(1000 / 60);
  }
  return 0;
}
