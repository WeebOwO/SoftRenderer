#pragma once

#include <functional>
#include <unordered_map>

#include "other/math.h"

struct ShaderContext {
    std::unordered_map<int, float> varyingFloat; // 浮点数 varying 列表
    std::unordered_map<int, Vec2f> varyingVec2f; // 二维矢量 varying 列表
    std::unordered_map<int, Vec3f> varyingVec3f; // 三维矢量 varying 列表
    std::unordered_map<int, Vec4f> varyingVec4f; // 四维矢量 varying 列表
    void                           Clear();
};

struct VertexAttrib {
    Vec3f pos;
    Vec3f normal;
    Vec2f uv;
};

struct Vertex {
    ShaderContext context;
    float         rhw;
    Vec4f         pos;
    Vec2f         spf;
    Vec2i         spi;
};

using VertexShader = std::function<Vec4f(VertexAttrib& vertexInput, ShaderContext& output)>;
using PixelShader  = std::function<Vec4f(ShaderContext& input)>;
