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

    Vec3f eyePos = {0, 0, 2};
    Vec3f eyeAt = {0, 0, 0};
    Vec3f eyeUp = {0, 1, 0};
    Vec3f lightDir = {1, 1, 0.85};
    float perspective = 3.1415926f * 0.5f;

    // 变换矩阵
    Mat4x4f matModel = matrix_set_rotate(0, 1, 0, 0);
    Mat4x4f matView = matrix_set_lookat(eyePos, eyeAt, eyeUp);
    Mat4x4f matProj =
        matrix_set_perspective(perspective, 9.0 / 6.0, 1.0, 500.0f);
    Mat4x4f mvp = matModel * matView * matProj;
    Mat4x4f matModelIt = matrix_invert(matModel).Transpose();

    renderer.SetVertexShader(
        [&](VertexAttrib vsInput, ShaderContext &output) -> Vec4f {
            Vec4f pos = vsInput.pos.xyz1() * mvp;
            Vec3f posWorld = (vsInput.pos.xyz1() * matModel).xyz();
            Vec3f eyeDir = eyePos - posWorld;
            output.varyingVec2F[VARYING_UV] = vsInput.uv;
            output.varyingFloat[VARYING_DEPTH] = vsInput.pos.z;
            output.varyingVec3F[VARYING_EYE] = eyeDir;
            return pos;
        });

    renderer.SetPixelShader([&](ShaderContext &input) {
        Vec2f uv = input.varyingVec2F[VARYING_UV];
        Vec3f eyeDir = input.varyingVec3F[VARYING_EYE];
        Vec4f baseColor = model.diffuse(uv);
        Vec3f l = vector_normalize(lightDir);
        Vec3f normal = (model.normal(uv).xyz1() * matModelIt).xyz();
        Vec3f r = vector_normalize(normal * vector_dot(normal, l) * 2.0f - l);
        float s = model.Specular(uv);
        float p = Saturate(vector_dot(r, eyeDir));
        float specIntensity = Saturate(pow(p, s * 10) * 0.05);

        float diffuseIntensity = vector_dot(l, normal);
        float ambientIntensity = 0.1;
        // test color
        //    Vec4f ambient_color = ambientIntensity * baseColor;
        //    Vec4f diffuse_color = diffuseIntensity * baseColor;
        //    Vec4f specular_color = specIntensity * baseColor;
        Vec4f outputColor =
            (diffuseIntensity + ambientIntensity + specIntensity) * baseColor;
        return vector_clamp(outputColor);
    });

    std::array<VertexAttrib, 3> vsInputs;

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
                vsInputs[j].pos = model.vert(i, j);
                vsInputs[j].uv = model.uv(i, j);
                vsInputs[j].normal = model.normal(i, j);
            }
            renderer.DrawPrimitive(vsInputs);
        }
        renderer.RenderPresent();
        SDL_Delay(1000 / 60);
    }
    return 0;
}
