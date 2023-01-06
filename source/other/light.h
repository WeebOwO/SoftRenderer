#pragma once

#include "math.h"

class BasicLight {
public:
    BasicLight(const Vec3f& lightPos, const Vec3f& lightColor)
        : m_lightPos(lightPos), m_lightColor(lightColor) {}
    [[nodiscard]] Vec3f GetLightPos() const { return m_lightPos; }
    [[nodiscard]] Vec3f GetLightColor() const { return m_lightColor; }
    virtual Vec3f       GetLightDir() { return m_lightPos; }

private:
    Vec3f m_lightPos{1.0f, 1.0f, 1.0f};
    Vec3f m_lightColor{1.0f, 1.0f, 1.0f};
};

class DirectionalLight : public BasicLight {
public:
    [[nodiscard]] Vec3f GetLightDir() override { return m_lightDir; }
    DirectionalLight(const Vec3f& lightPos, const Vec3f& lightColor, const Vec3f& lightDir)
        : BasicLight(lightPos, lightColor), m_lightDir(lightDir) {}

private:
    Vec3f m_lightDir{1, 1, 0.85};
};