#pragma once

#include <functional>
#include <vector>

class Shader {
private:
    std::function<void(std::vector<float>)> m_vertex_shader;
    std::function<void(std::vector<float>)> m_fragment_shader;
    
};