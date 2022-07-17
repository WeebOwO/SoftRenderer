#include <vector>

#include "mesh.hpp"
#include "renderer.hpp"
#include "math.hpp"

static std::vector<std::vector<float>> vertexbuffer = {
    //位置
    {240.0f, 100.0f, 255, 0, 0 },
    {150.f, 250.0f, 0, 255, 0 },
    {330.f, 250.0f, 0, 0, 255 },
};

static std::vector<std::vector<int>> indexbuffer = { 
    {0, 1, 2}
};

int main(int argc, char* argv[]) {
    Renderer renderer(600, 600);
    StaticMesh mesh(vertexbuffer, indexbuffer);
    renderer.run(mesh);
    return 0;
}