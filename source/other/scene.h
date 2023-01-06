#pragma once

#include <memory>
#include <vector>

#include "light.h"
#include "model.h"

class Scene {
public:
    void AddModel(const std::shared_ptr<Model>& model) { m_models.emplace_back(model); };
    void AddLight(const std::shared_ptr<BasicLight>& basicLight) {
        m_lights.emplace_back(basicLight);
    };
    [[nodiscard]] auto GetModels() { return m_models; }
    [[nodiscard]] auto GetLights() const { return m_lights; }

private:
    std::vector<std::shared_ptr<Model>>      m_models;
    std::vector<std::shared_ptr<BasicLight>> m_lights;
};