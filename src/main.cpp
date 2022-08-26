#include "renderer.h"

extern Vertex vs_shader(Vertex a);

std::vector<Vertex> vertex_buffer = {
    //齐次坐标                //颜色rgba 
    {{-1.0f, 0.0f, 2.0f, 1.0f}, {255, 0, 0, 255}},
    {{0.0f, 1.0f, 2.0f, 1.0f}, {0, 255, 0, 255}},
    {{1.0f, 0.0f, 2.0f, 1.0f}, {0, 0, 255, 255}},
};

std::vector<std::vector<int>> index_buffer = {
    {0, 1, 2}
};

WindowCreateInfo window_info = { 600, 600, "CoreEegine" };
int main(int argc, char* argv[]) {
    Buffer buffer(vertex_buffer, index_buffer);
    Window window(window_info);
    Shader shader(vs_shader);
    Renderer renderer(&window, &shader);
    renderer.run(buffer);
    return 0;
}