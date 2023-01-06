#pragma once

#include <array>
#include <span>
#include <unordered_map>

#include "other/bitmap.h"
#include "other/math.h"
#include "other/scene.h"
#include "shader.h"

class SDL_Renderer;
class SDL_Window;
class SDL_Texture;

struct WindowInfo {
    const char* title{"HardCore"};
    int         x{0};
    int         y{0};
    int         width{900};
    int         height{600};
};

class Renderer {
public:
    void RenderPresent();
    void DrawPrimitive(std::span<VertexAttrib, 3> vertexAttributes);
    void RenderClear();
    void RenderScene(Scene& scene);
    void DrawPixel(int x, int y, const Vec4f& color);
    void Resize(int width, int height);
    void ResizeDepthBuffer(int width, int height);
    void ResizeFrameBuffer(int width, int height);
    void SetVertexShader(VertexShader vertex_shader);
    void SetPixelShader(PixelShader pixel_shader);
    Renderer() = delete;
    explicit Renderer(const WindowInfo& windowInfo);
    ~Renderer();

private:
    ShaderContext BarycentricInterplate(std::span<Vertex, 3> vertices, const Vec3f& barycentric);

private:
    // 只是用来管理窗口的运行环境
    int                m_windowWidth{900};
    int                m_windowHeight{600};
    SDL_Renderer*      m_renderer{nullptr};
    SDL_Window*        m_window{nullptr};
    SDL_Texture*       m_swapTexture{nullptr};
    uint32_t*          m_frameBuffer;
    std::vector<float> m_depthBuffer;
    VertexShader       m_vertexShader;
    PixelShader        m_pixelShader;
};