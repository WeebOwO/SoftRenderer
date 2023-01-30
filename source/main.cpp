#include "renderer.h"

int main() {
    auto       model      = std::make_shared<Model>("../obj/diablo3_pose.obj");
    WindowInfo windowInfo = {"Core", 0, 0, 900, 600};
    Renderer   renderer   = Renderer(windowInfo);
    Scene      scene;
    Vec3f      lightPos   = {1, 1, 0.85};
    Vec3f      lightColor = {1, 1, 1};
    Vec3f      lightDir   = {1, 1, 0.85};
    scene.AddLight(std::make_shared<DirectionalLight>(lightPos, lightColor, lightDir));
    scene.AddModel(model);
    renderer.RenderScene(scene);
    return 0;
}
