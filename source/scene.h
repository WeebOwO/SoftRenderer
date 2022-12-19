#pragma once

#include <vector>

class scene {

};


//  for (int k = 0; k < 3; k++) {
//    Vertex& vertex = vertices[k];
//
//    // 清空上下文 varying 列表
//    vertex.context_.varying_float.clear();
//    vertex.context_.varying_vec2f.clear();
//    vertex.context_.varying_vec3f.clear();
//    vertex.context_.varying_vec4f.clear();
//
//    // 运行顶点着色程序，返回顶点坐标
//    vertex.pos_ = m_vertex_shader_(k, vertex.context_);
//
//    // 简单裁剪，任何一个顶点超过 CVV 就剔除
//    float w = vertex.pos_.w;
//
//    // 这里图简单，当一个点越界，立马放弃整个三角形，更精细的做法是
//    // 如果越界了就在齐次空间内进行裁剪，拆分为 0-2 个三角形然后继续
//    if (w == 0.0f) return ;
//    if (vertex.pos_.z < 0.0f || vertex.pos_.z > w) return ;
//    if (vertex.pos_.x < -w || vertex.pos_.x > w) return ;
//    if (vertex.pos_.y < -w || vertex.pos_.y > w) return ;
//
//    // 计算 w 的倒数：Reciprocal of the Homogeneous W
//    vertex.rhw_ = 1.0f / w;
//
//    // 齐次坐标空间 /w 归一化到单位体积 cvv
//    vertex.pos_ *= vertex.rhw_;
//
//    // 计算屏幕坐标
//    vertex.spf_.x = (vertex.pos_.x + 1.0f) * width * 0.5f;
//    vertex.spf_.y = (1.0f - vertex.pos_.y) * height * 0.5f;
//
//    // 整数屏幕坐标：加 0.5 的偏移取屏幕像素方格中心对齐
//    vertex.spi_.x = (int)(vertex.spf_.x + 0.5f);
//    vertex.spi_.y = (int)(vertex.spf_.y + 0.5f);
//
//    // 更新外接矩形范围
//    if (k == 0) {
//      min_x = max_x = Between(0, width - 1, vertex.spi_.x);
//      min_y = max_y = Between(0, height - 1, vertex.spi_.y);
//    }
//    else {
//      min_x = Between(0, width - 1, Min(min_x, vertex.spi_.x));
//      max_x = Between(0, width - 1, Max(max_x, vertex.spi_.x));
//      min_y = Between(0, height - 1, Min(min_y, vertex.spi_.y));
//      max_y = Between(0, height - 1, Max(max_y, vertex.spi_.y));
//    }
//  }