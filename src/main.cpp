#include "renderer.h"

extern Vertex vs_shader(Vertex a);

std::vector<Vertex> vertex_buffer = {
    //齐次坐标                //颜色rgba 
    {{-1.0f, -1.0f, -1.0f, 1.0f}, {255, 0, 0, 255}},
    {{-1.0f, 1.0f, -1.0f, 1.0f}, {0, 255, 0, 255}},
    {{1.0f, 1.0f, -1.0f, 1.0f}, {0, 0, 255, 255}},
    {{1.0f, -1.0f, -1.0f, 1.0f}, {0, 0, 255, 255}},

    {{-1.0f, -1.0f, 1.0f, 1.0f}, {255, 0, 0, 255}},
    {{-1.0f, 1.0f, 1.0f, 1.0f}, {0, 255, 0, 255}},
    {{1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 255, 255}},
    {{1.0f, -1.0f, 1.0f, 1.0f}, {0, 0, 255, 255}},
};

std::vector<std::vector<int>> index_buffer = {
    {0, 1, 2},
    {0, 3, 2},

    {4, 6, 7},
    {4, 5, 6},

    {1, 4, 0},
    {1, 4, 5},

    {2, 3, 7},
    {2, 6, 7},

    {0, 3, 4},
    {3, 4, 7},

    {1, 2, 5},
    {2, 5, 6}

};

WindowCreaterInfo window_info = {
    "CoreEngine",
    600,
    600
};

int main(int argc, char* argv[]) {
    //着色器设置
    Shader shader(vs_shader);
    //各种缓冲区设置
    Buffer buffer(vertex_buffer, index_buffer);
    //渲染器启动
    Renderer renderer(window_info, &shader);
    renderer.run(buffer);
    return 0;
} 