#pragma once
#include <vector>
#include "shader.h"

struct Buffer {
    std::vector<Vertex> verter_buffer;
    std::vector<std::vector<int>> index_buffer;
    std::vector<std::vector<float>> depth_buffer;
};
  