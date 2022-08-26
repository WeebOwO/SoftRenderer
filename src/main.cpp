#include <vector>

#include "renderer.hpp"
#include "shader.hpp"

std::vector<Vertex> vertex_buffer = {
    //齐次坐标                //颜色rgba 
    {{-1.0f, 0.0f, 2.0f, 1.0f}, {255, 0, 0, 255}},
    {{0.0f, 1.0f, 2.0f, 1.0f}, {0, 255, 0, 255}},
    {{1.0f, 0.0f, 2.0f, 1.0f}, {0, 0, 255, 255}},
};


std::vector<std::vector<int>> index_buffer = {
    {0, 1, 2}
};

int main(int argc, char* argv[]) {
    Renderer renderer(600, 600);
    renderer.set_vertexshader(vs_shader);
    renderer.run(vertex_buffer, index_buffer);
    return 0;
}