#pragma once
#include <vector>
#include <utility>
#include <iostream>
#include <format>

struct StaticMesh {
    std::vector<std::vector<float>> vertexbuffer;
    std::vector<std::vector<int>> indexbuffer;

    //构造函数相关
    StaticMesh(const std::vector<std::vector<float>>& vertexbuffer, const std::vector<std::vector<int>>& indexbuffer) : 
        vertexbuffer(vertexbuffer), indexbuffer(indexbuffer) {}

    StaticMesh(std::vector<std::vector<float>>&& vertexbuffer, std::vector<std::vector<int>>&& indexbuffer) :
        vertexbuffer(std::move(vertexbuffer)), indexbuffer(std::move(indexbuffer)) {
        std::cout << "Move construct!\n";
    }
    //模型数据统计

};  