#include <vector>

#include "mesh.hpp"
#include "renderer.hpp"
#include "math.hpp"
#include "shader.hpp"

std::vector<Vertex> vertex_buffer = {
    //坐标                //颜色rgba 
    {{300.0f, 150.0f, 1.0f}, {255, 0, 0, 255}},
    {{1.0f, 0.0f, 1.0f}, {0, 255, 0, 255}},
    {{1.0f, 0.0f, 1.0f}, {0, 0, 255, 255}},
};

std::vector<std::vector<int>> index_buffer = {
    {0, 1, 2}
};


int main(int argc, char* argv[]) {
    Renderer renderer(600, 600);
    return 0;
}